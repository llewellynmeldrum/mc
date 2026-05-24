#include "ChunkStorage.hpp"
#include "Chunk.hpp"


ChunkStore::ChunkStore(const Chunk& chunk){
    buf.resize(CHUNK_SIZE);
    std::ranges::copy(chunk.data,buf.begin());
}
ChunkStore::ChunkStore(const Chunk* chunk){
    buf.resize(CHUNK_SIZE);
    std::ranges::copy(chunk->data,buf.begin());
}
ChunkStore::ChunkStore(){
    buf.resize(CHUNK_SIZE);
}
