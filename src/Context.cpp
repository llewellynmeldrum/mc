
#include "Chunk.hpp"
#include "ChunkConcurrency.hpp"
#include "ChunkHelpers.hpp"
#include "CoordTypes.hpp"
#include "DebugFormat.hpp"
#include "DebugFormatSpecializations.hpp"

#include "Context.hpp"

#include "Profiler.hpp"
#include "Logger.hpp"
#include <algorithm>

using namespace glm;
void Context::setupContext() {
    program_epoch_ns = get_current_ns();
    LOG_EXPR(this);
    win.setupWindow(static_cast<void*>(this));
    input.setupInput(win.ptr);
    time.setupTimer(
        "frame",
        "input",
        "update",
        "enqueueGen",
        "drainGen",
        "enqueueMesh",
        "drainMesh",
        "draw",
        "render"
    );
    cam.setupCamera();
    rend.setupRenderer();
    ui.setupDebugUI(win.ptr);
    world.setupWorld();
}
void Context::loop(){
    time.bench_start("frame");

    // INPUT
    {
        time.bench_start("input");
        input.poll();
        handleInputs(); 
        time.bench_end("input");
    }

    // UPDATE:
    {
        time.bench_start("update");
        update();
        time.bench_end("update");
    }

    // DRAWING
    {
        time.bench_start("draw");
        draw(); 
        ui.drawDebugUI(); 
        time.bench_end("draw");
    }

    {
        time.bench_start("render");
        ui.render();
        win.swapBuffers();
        time.bench_end("render");
    }
    time.bench_end("frame");
}

void Context::update() {
    time.bench_start("enqueueGen");
    enqueueGenerationJobs();
    time.bench_end("enqueueGen");

    time.bench_start("drainGen");
    drainAndUploadGenResults();
    time.bench_end("drainGen");

    time.bench_start("enqueueMesh");
    enqueueMeshingJobs();
    time.bench_end("enqueueMesh");

    time.bench_start("drainMesh");
    drainAndUploadMeshResults();
    time.bench_end("drainMesh");
}

std::vector<ivec3> Context::findChunksForGeneration(){
    std::vector<ivec3> res;
    auto chunkRange  = EachInRange(-SIMULATION_DIST, SIMULATION_DIST,
                                        -SIMULATION_DIST, SIMULATION_DIST,
                                        -SIMULATION_DIST, SIMULATION_DIST);

    for (const auto& [cx,cy,cz] : chunkRange){
        const ivec3 chunkCoord = toWorldChunkCoord(cam.pos);
        const ivec3 key = chunkCoord + ivec3{cx,cy,cz}; // dont you have to 
        const auto genStatus = world.chunkMap.queryGenStatus(key);
        if (genStatus.finishedAll == false){
            res.emplace_back(key);
        }
    }
    return res;
}

std::size_t Context::enqueueGenerationJobs(){
    const auto chunkWorldPosForGen = findChunksForGeneration();
//    LOG_DEBUG("Chunks in range found:{}",chunkWorldPosForGen.size());
    std::size_t res = 0;
    for (const ivec3& chunkWorldPos: chunkWorldPosForGen){

        auto genStatus = world.chunkMap.queryGenStatus(chunkWorldPos);
        if (genStatus.inProgress){
            continue; // already 
        }
        bool success = world.chunkMap.generator.genJobQueue.try_emplace(
            chunkWorldPos, 
            WORLD_SEED,
            world.genConfig
        );
        if (success){
            res++;
            world.chunkMap.genStatus.try_emplace(chunkWorldPos,ChunkGenStatus{.inProgress=true});
        }else{
//            LOG_DEBUG("FAILED TO ENQUEUE FOR GEN");

            break;
        }
    }
 //   if (res>0) LOG_DEBUG("enqueued_for_gen this frame:{}",res);
    return res;
}
// update

std::vector<MeshJob> Context::findMeshJobs(){

    auto in_frustum = [](const Frustum& frustum, const World& world) {
        return [frustum, &world](const WorldChunkCoord& chunk_coord) {
            return frustum.isAABBInside(world.chunkMap.getBoundingBox(chunk_coord));
        };
    };

    
    const auto camChunkCoord = toWorldChunkCoord(cam.pos);
    const auto candidateList = world.generatedChunkCoordsInRadius(camChunkCoord, RENDER_DIST, maxMeshesPerFrame);
    // TODO: 
    // Unify chunk map into a single ChunkEntry struct,
    // perhaps a separate map for pendingBlockWrites, since they can apply to ungenerated chunks,
    // Keep refining queue logic to make chunk meshing faster,
    // -> Consider a partial sort of in ranges meshes (nth element?)
    // Im currently only sorting the first N in the arbitrary evaluation order of the cartesian product.
    // Perhaps i make the loop itself expand out from the center, i.e instead of:
    // 123       526
    // 4C5   ->  1C3  (nearby chunks are checked first, leaving sorting uneccesary)
    // 678       847
    //
    // NOTE:
    // The enumeration of other chunks should also only start at the point of unmeshed chunks
    // This whole thing is a bit of a shitshow, is this really the right way to go over them?
    //

    std::vector<MeshJob> res;
    for (const auto candidate: candidateList){
        // TODO: to 4-5x reduce the size of a mesh jobs allocation, 
        // i can reduce the surrounding Chunks block storage to only contain the boundary blocks,
        // i.e the ones bordering the actual chunk in question.
        const auto surrounding = world.chunkMap.getSurroundingChunks(candidate);
        const auto* meta = world.chunkMap.getMetadata(candidate);
        const auto generationID = world.chunkMap.getLatestMeshRevisionID(candidate);
        const auto* chunk_ptr = world.chunkMap.getEntry(candidate);
        res.emplace_back(
            generationID,
            candidate,
            chunk_ptr,
            surrounding,
            meta,
            &rend.atlas
        );

    }
    return res;
}

std::size_t Context::enqueueMeshingJobs(){
    auto jobsForMeshing = findMeshJobs();

    std::size_t res = 0;
    for (auto& job: jobsForMeshing){
        if (rend.mesher.meshJobQueue.try_enqueue(std::move(job))){
            world.chunkMap.markMeshing(job.chunkCoord);
            res++;
        } else{
            // stop submitting this frame
            break;
        }
        //NOTE:
        // its possible that using an emplace method could be faster,
        // since we avoid constructing the object in case of lock contention.
        // Either way, we have to make the chunk entry lookup, so im unsure of the benefit here.
        // Cant be that bad, since they released unordered_map::emplace() which emplaces regardless
        // of insertion, and it took them until c++17 to fix that with try_emplace.
    }
    LOG_DEBUG("enqueued_for_meshing this frame:{}/{}",res,jobsForMeshing.size());
    return res;
}


std::vector<GenResult> drainGenResults(Queue<GenResult>& queue){
    constexpr std::size_t MaxGenUploadsPerFrame = 12;
    std::vector<GenResult> output;
    output.reserve(MaxGenUploadsPerFrame);
    for (std::size_t mesh_count = 0; mesh_count < MaxGenUploadsPerFrame; mesh_count++){
        std::optional<GenResult> result = queue.try_dequeue();
        if (!result){
            break; // give up this frame?
        }
        output.emplace_back(*result);
    }
    return output;
}

std::vector<MeshResult> drainMeshResultQueue(Queue<MeshResult>& queue){
    constexpr std::size_t MaxMeshUploadsPerFrame = 128;
    std::vector<MeshResult> output;
    output.reserve(MaxMeshUploadsPerFrame);
    constexpr i32 meshDequeueAttempts = 8;
    for (std::size_t mesh_count = 0; mesh_count < MaxMeshUploadsPerFrame; mesh_count++){
        for (i32 i =0; i<meshDequeueAttempts; i++){
            std::optional<MeshResult> meshData = queue.try_dequeue();
            if (!meshData){
                continue;
            }
            output.emplace_back(std::move(*meshData));
        }
    }

    return output;
}
std::size_t Context::drainAndUploadMeshResults(){
    std::size_t res =0;
    auto candidateMeshes = drainMeshResultQueue(rend.mesher.meshResultQueue);
    for (const auto& [candidateGen, chunkCoord, vertices, indices] : candidateMeshes){
        if (vertices.size()>0){
            rend.uploadMesh(chunkCoord, vertices, indices);
        } 
        chunksMeshed++;
        res++;
        world.chunkMap.markClean(chunkCoord);
    }
    return res;
}

std::size_t Context::drainAndUploadGenResults(){
    auto chunkDatas = drainGenResults(world.chunkMap.generator.genResultQueue);
    auto res = chunkDatas.size();
    for (const auto& newGen : chunkDatas){
        world.chunkMap.uploadGeneratedChunk(newGen);
    }
    return res;
}

void Context::draw() {
    rend.clear({ 0.25, 0.5, 0.85, 1.0 });
    ScopeTimer t_mesh_chunks{ "Chunk meshing", "chunk" };

    rend.draw(cam);
    static bool first_draw = true;
    if (first_draw) {
        LOG_DEBUG("Finished first draw");
        first_draw = false;
    }
}

void Context::handleInputs() {
    if (input.getKey(KEY_ESCAPE) == KeyState::Held) {
        win.scheduleClose();
        return;
    }
    f32 dt = time.dt_s;
    // dt = 1/60
    for (auto& [key, cd] : input.keyRepeatCooldown) {
        if (cd > 0.0f) {
            cd -= dt;
        }
    }
    f32 scaled_dt = dt * 60;

    if (input.mousepos != input.prevmousepos) {
        const vec2 diff = input.prevmousepos - input.mousepos;
        cam.rotateByMouse(diff, scaled_dt);
    }

    if (input.getKey(KEY_T) == KeyState::Held) {
        // toggle wireframe renderer
        if (input.keyRepeatCooldown.at(KEY_T) <= 0.0f) {
            rend.debug.wireframe = !rend.debug.wireframe;
            input.keyRepeatCooldown[KEY_T] = 0.1f;
        }
    }
    if (input.getKey(KEY_R) == KeyState::Held) {
        // remesh current chunk
        if (input.keyRepeatCooldown.at(KEY_R) <= 0.0f) {
            rend.debug.wireframe = !rend.debug.wireframe;
            input.keyRepeatCooldown[KEY_R] = 0.1f;
        }
    }
    if (input.getKey(KEY_LEFT_SHIFT) == KeyState::Held) {
        cam.moveSpeed = Camera::SPRINT_MOVESPEED;
    } else {
        cam.moveSpeed = Camera::BASE_MOVESPEED;
    }
    if (input.getKey(KEY_W) == KeyState::Held) {
        cam.move(Direction::FORWARD, scaled_dt);
    }
    if (input.getKey(KEY_S) == KeyState::Held) {
        cam.move(Direction::BACKWARD, scaled_dt);
    }
    if (input.getKey(KEY_A) == KeyState::Held) {
        cam.move(Direction::LEFT, scaled_dt);
    }
    if (input.getKey(KEY_D) == KeyState::Held) {
        cam.move(Direction::RIGHT, scaled_dt);
    }
    if (input.getKey(KEY_SPACE) == KeyState::Held) {
        cam.move(Direction::UP, scaled_dt);
    }
    if (input.getKey(KEY_E) == KeyState::Held) {
        cam.move(Direction::UP, scaled_dt);
    }
    if (input.getKey(KEY_Q) == KeyState::Held) {
        cam.move(Direction::DOWN, scaled_dt);
    }

    if (input.getKey(KEY_LEFT) == KeyState::Held) {
        cam.rotate(Direction::LEFT, scaled_dt);
    }
    if (input.getKey(KEY_RIGHT) == KeyState::Held) {
        cam.rotate(Direction::RIGHT, scaled_dt);
    }
    if (input.getKey(KEY_UP) == KeyState::Held) {
        cam.rotate(Direction::UP, scaled_dt);
    }
    if (input.getKey(KEY_DOWN) == KeyState::Held) {
        cam.rotate(Direction::DOWN, scaled_dt);
    }
    input.prevmousepos = input.mousepos;
}
