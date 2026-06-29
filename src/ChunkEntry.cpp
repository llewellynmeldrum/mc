#include "ChunkEntry.hpp"
#include "CoordTypes.hpp"
#include "FormatSpecs.hpp"
#include "DebugUI.hpp"


#include "Engine.hpp"
static Engine* sim{};

//TODO: change all but mark_mesh_dirty to accept chunkState instead of ChunkEntry
void gen_enqueue(ChunkState* s) {
   1; // zorp
}
void gen_dequeue(ChunkState* e) {
    assert_eq(e->gen,GenState::on_queue);
    assert_eq(e->mesh,MeshState::awaiting_generation);
    e->mesh = MeshState::ready_for_enqueue;
    e->gen = GenState::done;
}

void mesh_enqueue(ChunkState* e) {
    assert_eq(e->gen,GenState::done);
    assert(e->mesh == MeshState::ready_for_enqueue || e->mesh == MeshState::done);

    e->mesh = MeshState::on_queue;
}

void mesh_dequeue(ChunkState* e) {
    assert_eq(e->gen,GenState::done);
    assert_eq(e->mesh,MeshState::on_queue);
    e->mesh = MeshState::done;
}

void init_state_transition_logger(Engine* _sim){
    sim=_sim;
}

void transition_logger(const ChunkState& before, const ChunkState& after){
    if (before.mesh != after.mesh){
        std::string s = std::format("M: {} -> {}",before.mesh,after.mesh);
        log_to_chunk(before.coord,"{}",s);
    }
    if (before.gen != after.gen){
        std::string s = std::format("G: {} -> {}",before.gen,after.gen);
        log_to_chunk(before.coord,"{}",s);
    }
}
