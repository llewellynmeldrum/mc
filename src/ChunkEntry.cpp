#include "ChunkEntry.hpp"
#include "CoordTypes.hpp"
#include "FormatSpecs.hpp"
#include "DebugUI.hpp"


#include "Engine.hpp"

using PipelineState::pending;
using PipelineState::ready_for_enqueue;
using PipelineState::on_queue;
using PipelineState::done;
//TODO: change all but mark_mesh_dirty to accept chunkState instead of ChunkEntry
//
void gen_enqueue(ChunkState* e) {
    assert(e->gen == ready_for_enqueue || e->gen == done);
    e->gen = on_queue;
}

void gen_dequeue(ChunkState* e) {
    assert_eq(e->gen,on_queue);
    assert_eq(e->mesh,pending);
    e->mesh = ready_for_enqueue;
    e->gen = done;
}
void delete_gen(ChunkState* e) {
    assert_eq(e->gen,done);
    e->gen = ready_for_enqueue;
}

void mesh_enqueue(ChunkState* e) {
    assert_eq(e->gen,done);
    assert(e->mesh == ready_for_enqueue || e->mesh == done);
    e->mesh = on_queue;
}

void mesh_dequeue(ChunkState* e) {
    assert_eq(e->gen,done);
    assert_eq(e->mesh,on_queue);
    e->mesh = done;
}

void delete_mesh(ChunkState* e) {
    assert_eq(e->gen,done);
    assert(e->mesh == done);

    e->mesh = ready_for_enqueue;
}

void transition_logger(const ChunkState& before, const ChunkState& after){
    if (before.mesh != after.mesh){
        std::string s = std::format("M: {} -> {}",before.mesh,after.mesh);
        log_to_chunk("mesh_state_change", before.coord,"{}",s);
        //     TODO: FIxxx
    }
    if (before.gen != after.gen){
        std::string s = std::format("G: {} -> {}",before.gen,after.gen);
        log_to_chunk("gen_state_change", before.coord,"{}",s);
    }
}


// NOTE: ===========
// NOTE: GEN STUFF
// NOTE: ===========

bool RevisionState::is_dirty() const noexcept{
    return loaded != target;
}
bool RevisionState::is_clean() const noexcept{
    return loaded == target;
}

bool RevisionState::qualifies_for_enqueue(const PipelineState& state) const noexcept {
    const bool target_is_newer_than_inflight = (target > inflight);
    const bool ready = state == ready_for_enqueue;
    const bool dirty_done = state == done && is_dirty();
    return target_is_newer_than_inflight && (ready || dirty_done);
}

bool RevisionState::qualifies_for_dequeue(const PipelineState& state) const noexcept{
    return state == on_queue;
}

bool RevisionState::is_candidate_newer_than_loaded(GenRevisionID candidate_id) const noexcept{
    return candidate_id > loaded;
}

void RevisionState::mark_dirty() noexcept{
    target++;
}


void ChunkEntry::mark_mesh_deleted(){
    state_transition(delete_mesh);
    mesh_revision.loaded = 0;
}
void ChunkEntry::mark_gen_deleted(){
    state_transition(delete_gen);
    gen_revision.loaded = 0;
}
bool ChunkEntry::qualifies_for_mesh_enqueue()const noexcept{
    return state.gen == done && mesh_revision.qualifies_for_enqueue(state.mesh);
}
bool ChunkEntry::qualifies_for_mesh_dequeue()const noexcept{
    return mesh_revision.qualifies_for_dequeue(state.mesh);
}
bool ChunkEntry::qualifies_for_gen_enqueue()const noexcept{
    return gen_revision.qualifies_for_enqueue(state.gen);
}
bool ChunkEntry::qualifies_for_gen_dequeue()const noexcept{
    return gen_revision.qualifies_for_dequeue(state.gen);
}
