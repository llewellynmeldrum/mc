#include "ChunkEntry.hpp"
#include "CoordTypes.hpp"
#include "FormatSpecs.hpp"
#include "DebugUI.hpp"


#include "Engine.hpp"

//TODO: change all but mark_mesh_dirty to accept chunkState instead of ChunkEntry
void gen_enqueue(ChunkState* e) {
    assert(e->gen == GenState::ready_for_enqueue || e->gen == GenState::done);

    e->gen = GenState::on_queue;
    // This is a NOP that exists for logging/symmetry purposes.
}
void gen_dequeue(ChunkState* e) {
    assert_eq(e->gen,GenState::on_queue);
    assert_eq(e->mesh,MeshState::awaiting_generation);
    e->mesh = MeshState::ready_for_enqueue;
    e->gen = GenState::done;
}
void delete_gen(ChunkState* e) {
    assert_eq(e->gen,GenState::done);

    e->gen = GenState::ready_for_enqueue;
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

void delete_mesh(ChunkState* e) {
    assert_eq(e->gen,GenState::done);
    assert(e->mesh == MeshState::done);

    e->mesh = MeshState::ready_for_enqueue;
}

void transition_logger(const ChunkState& before, const ChunkState& after){
    if (before.mesh != after.mesh){
        std::string s = std::format("M: {} -> {}",before.mesh,after.mesh);
        log_to_chunk("mesh_state_change", before.coord,"{}",s);
    }
    if (before.gen != after.gen){
        std::string s = std::format("G: {} -> {}",before.gen,after.gen);
        log_to_chunk("gen_state_change", before.coord,"{}",s);
    }
}


// NOTE: ===========
// NOTE: GEN STUFF
// NOTE: ===========

bool ChunkEntry::is_gen_dirty() const noexcept{
    return loaded_gen_revision != target_gen_revision;
}
bool ChunkEntry::is_gen_clean() const noexcept{
    return loaded_gen_revision == target_gen_revision;
}
bool ChunkEntry::qualifies_for_gen_enqueue()const noexcept {
    const bool target_is_newer_than_inflight = (target_gen_revision > inflight_gen_revision);
    const bool ready = state.gen == GenState::ready_for_enqueue;
    const bool dirty_done = state.gen == GenState::done && is_gen_dirty();
    return target_is_newer_than_inflight && (ready || dirty_done);
}
bool ChunkEntry::qualifies_for_gen_dequeue() const noexcept{
    return state.gen==GenState::on_queue;
}

bool ChunkEntry::is_candidate_gen_newer_than_loaded(GenRevisionID candidate_gen_revision_id) const noexcept{
    return candidate_gen_revision_id > loaded_gen_revision;
}

void ChunkEntry::mark_gen_deleted(){
    state_transition(delete_gen);
    inflight_gen_revision = 0;
    loaded_gen_revision = 0;
}
bool ChunkEntry::_mark_gen_dirty() noexcept{
    return target_gen_revision++;
}


// NOTE: ===========
// NOTE: MESH STUFF
// NOTE: ===========

bool ChunkEntry::is_mesh_dirty()const noexcept{
    return loaded_mesh_revision!=target_mesh_revision;
}
bool ChunkEntry::is_mesh_clean()const noexcept{
    return loaded_mesh_revision==target_mesh_revision;
}

bool ChunkEntry::qualifies_for_mesh_enqueue()const noexcept{
    const bool target_is_newer_than_inflight = (target_mesh_revision > inflight_mesh_revision);
    const bool gen_done = state.gen == GenState::done;
    const bool ready = state.mesh==MeshState::ready_for_enqueue;
    const bool dirty_done = state.mesh==MeshState::done && is_mesh_dirty();
    return target_is_newer_than_inflight && gen_done && (ready || dirty_done);
}
bool ChunkEntry::qualifies_for_mesh_dequeue()const noexcept {
    return state.mesh == MeshState::on_queue;
}


// WARNING: DO NOT USE THIS FUNCTION BY ITSELF! SHOULD ONLY BE USED FROM DIRECTOR
void ChunkEntry::_mark_mesh_dirty(std::string_view reason)noexcept{
    log_to_chunk("mesh_endirtying",state.coord, 
                 "Mesh dirtied ({}->{}). Reason:{}",
                 target_mesh_revision,
                 target_mesh_revision+1,
                 reason);
    target_mesh_revision++;
}

bool ChunkEntry::is_candidate_mesh_newer_than_loaded(MeshRevisionID candidate_mesh_revision_id) const noexcept{
    return candidate_mesh_revision_id > loaded_mesh_revision;
}
void ChunkEntry::mark_mesh_deleted(){
    state_transition(delete_mesh);
    inflight_mesh_revision = 0;
    loaded_mesh_revision = 0;
}
