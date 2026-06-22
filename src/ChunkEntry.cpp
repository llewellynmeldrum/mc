#include "ChunkEntry.hpp"
#include "CoordTypes.hpp"
#include "FormatSpecs.hpp"
#include "DebugUI.hpp"


#include "Simulation.hpp"
static Simulation* sim{};

void gen_enqueue(ChunkEntry* s) {
   1; // zorp
}
void gen_dequeue(ChunkEntry* e) {
    assert_eq(e->state.gen.stage,GenStage::on_queue);
    assert_eq(e->state.mesh.stage,MeshStage::awaiting_generation);
    e->state.mesh.stage = MeshStage::ready_for_enqueue;
    e->state.gen.stage = GenStage::done;
}

void mark_mesh_dirty(ChunkEntry* e) {
//    assert_eq(e->state.gen.stage,GenStage::done);
//    assert_eq(e->state.mesh.stage,MeshStage::done);
    e->state.mesh.dirty = true;
    e->target_mesh_revision++;
    assert_neq(e->target_mesh_revision,e->loaded_mesh_revision);
}
void mark_mesh_clean(ChunkEntry* e) {
//    assert_eq(e->state.gen.stage,GenStage::done);
//    assert_eq(e->state.mesh.stage,MeshStage::done);
    assert_eq(e->target_mesh_revision,e->loaded_mesh_revision);
    e->state.mesh.dirty = false;
    e->target_mesh_revision++;
}

void mesh_enqueue(ChunkEntry* e) {
    assert_eq(e->state.gen.stage,GenStage::done);
    assert(e->state.mesh.stage == MeshStage::ready_for_enqueue || e->state.mesh.stage == MeshStage::done);

    e->scheduled_mesh_revision=e->target_mesh_revision;
    e->state.mesh.stage = MeshStage::on_queue;
}

void mesh_dequeue(ChunkEntry* e) {
    assert_eq(e->state.gen.stage,GenStage::done);
    assert_eq(e->state.mesh.stage,MeshStage::on_queue);
    e->state.mesh.stage = MeshStage::done;
}
void init_state_transition_logger(Simulation* _sim){
    sim=_sim;
}

void transition_logger(const ChunkState& before, const ChunkState& after){
    if (before.mesh.dirty != after.mesh.dirty){
        std::string dirty_state = before.mesh.dirty ? "dirty -> clean" : "clean -> dirty";
        log_to_chunk(before.coord,"M: {}",dirty_state);
    }
    if (before.gen.dirty != after.gen.dirty){
        std::string dirty_state = before.gen.dirty ? "dirty -> clean" : "clean -> dirty";
        log_to_chunk(before.coord,"M: {}",dirty_state);
    }

    if (before.mesh.stage != after.mesh.stage){
        std::string s = std::format("M: {} -> {}",before.mesh.stage,after.mesh.stage);
        log_to_chunk(before.coord,"G: {}",s);
    }
    if (before.gen.stage != after.gen.stage){
        std::string s = std::format("M: {} -> {}",before.gen.stage,after.gen.stage);
        log_to_chunk(before.coord,"G: {}",s);
    }
}
