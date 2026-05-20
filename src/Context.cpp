
#include "Chunk.hpp"
#include "ChunkHelpers.hpp"
#include "DebugFormat.hpp"
#include "DebugFormatSpecializations.hpp"

#include "Context.hpp"

#include "Profiler.hpp"
#include "Logger.hpp"
#include <algorithm>

void Context::setupContext() {
    program_epoch_ns = get_current_ns();
    LOG_EXPR(this);
    win.setupWindow(static_cast<void*>(this));
    input.setupInput(win.ptr);
    time.setupTimer();
    cam.setupCamera();
    rend.setupRenderer();
    ui.setupDebugUI(win.ptr);
    world.setupWorld();
}
std::vector<ivec3> Context::findChunksForGeneration(){
    std::vector<ivec3> res;
    auto chunkRange  = EachInRange(-SIMULATION_DIST, SIMULATION_DIST,
                                        -SIMULATION_DIST, SIMULATION_DIST,
                                        -SIMULATION_DIST, SIMULATION_DIST);

    for (const auto& [x,y,z] : chunkRange){
        const ivec3 camWorld = World::worldToChunkCoord(cam.pos);
        const ivec3 key = camWorld + ivec3{x,y,z}; // dont you have to 
        const auto genStatus = world.chunkMap.queryGenStatus(key);
        if (genStatus.finishedAll == false){
            res.emplace_back(key);
        }
    }
    return res;
}

std::size_t Context::enqueueGenerationJobs(){
    const auto chunkWorldPosForGen = findChunksForGeneration();
    LOG_DEBUG("Chunks in range found:{}",chunkWorldPosForGen.size());
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
        }else{
            LOG_DEBUG("FAILED TO ENQUEUE FOR GEN");

            break;
        }
    }
    if (res>0) LOG_DEBUG("enqueued_for_gen this frame:{}",res);
    return res;
}
// update

std::vector<ChunkView> Context::findChunksForMeshing(){
    auto needs_meshing = [](const World& world){
        return [&world](const ChunkView & pair) {
            const auto& [chunk_coord, ptr] = pair;
            return world.chunkMap.isDirty(chunk_coord);
        };
    };

    auto in_frustum = [](const Frustum& frustum, const World& world) {
        return [frustum, &world](const ChunkView & pair) {
            const auto& [chunk_coord, ptr] = pair;
            return frustum.isAABBInside(*world.chunkMap.getBoundingBox(chunk_coord));
        };
    };

    
    return  world.chunksInRadius(World::worldToChunkCoord(cam.pos), RENDER_DIST) 
        | std::views::filter(needs_meshing(world)) 
//      | std::views::filter(in_frustum(cam.getFrustum(), world))
//       BUG: i have not verified that the frustum is working properly
        | std::ranges::to<std::vector<ChunkView>>();
}

std::size_t Context::enqueueMeshingJobs(){
    auto chunksForMeshing = findChunksForMeshing();

    std::size_t res =0;
    for (const auto& [chunk_pos, chunk_ptr]: chunksForMeshing){
        if (world.chunkMap.isMeshing(chunk_pos)){
            continue;
        }
        // TODO: to (seriously, like 4-5x) reduce the size of a mesh jobs allocation, 
        // i can reduce the surrounding Chunks block storage to only contain the boundary blocks,
        // i.e the ones bordering the actual chunk in question.
        const auto surrounding = world.chunkMap.getSurroundingChunks(chunk_pos);
        const auto* meta = world.chunkMap.getMetadata(chunk_pos);
        if (rend.mesher.meshJobQueue.try_emplace(
            chunk_pos,
            chunk_ptr,
            surrounding,
            meta,
            &rend.atlas
        )){
            res++;
            world.chunkMap.markMeshing(chunk_pos);
        } else{
            // stop submitting this frame
            break;
        }
    }
    if (res>0) LOG_DEBUG("enqueued_for_meshing this frame:{}",res);
    return res;
}

std::vector<MeshResult> drainMeshResultQueue(Queue<MeshResult>& queue){
    constexpr std::size_t MaxMeshUploadsPerFrame = 12;
    std::vector<MeshResult> output;
    output.reserve(MaxMeshUploadsPerFrame);
    for (std::size_t mesh_count = 0; mesh_count < MaxMeshUploadsPerFrame; mesh_count++){
        std::optional<MeshResult> meshData = queue.try_dequeue();
        if (!meshData){
            break;
        }
        output.emplace_back(*meshData);
        const auto [chunk_pos, vertices, indices] = *meshData;
    }
    return output;
}

std::vector<GenResult> drainGenResults(Queue<GenResult>& queue){
    constexpr std::size_t MaxGenUploadsPerFrame = 12;
    std::vector<GenResult> output;
    output.reserve(MaxGenUploadsPerFrame);
    for (std::size_t mesh_count = 0; mesh_count < MaxGenUploadsPerFrame; mesh_count++){
        std::optional<GenResult> result = queue.try_dequeue();
        if (!result){
            break;
        }
        output.emplace_back(*result);
    }
    return output;
}

std::size_t Context::drainAndUploadMeshResults(){
    std::size_t res =0;
    auto candidateMeshes = drainMeshResultQueue(rend.mesher.meshResultQueue);
    for (const auto& [header, vertices, indices] : candidateMeshes){
        WorldChunkCoord chunkPos = toWorldChunkCoord(header.chunkCoord);
        rend.uploadMesh(chunkPos, vertices, indices);
        res++;
        world.chunkMap.markClean(header.chunkCoord);
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
void Context::update() {
    //1. handle generation and meshing jobs
    enqueueGenerationJobs();
    drainAndUploadGenResults();

    enqueueMeshingJobs();
    drainAndUploadMeshResults();
}

void Context::draw() {
    rend.clear({ 0.25, 0.5, 0.85, 1.0 });
    ScopeTimer t_mesh_chunks{ "Chunk meshing", "chunk" };

    rend.draw(cam.getViewMatrix(), cam.getProjectionMatrix());
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
