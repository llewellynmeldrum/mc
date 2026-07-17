
#include "ChunkEntry.hpp"
#include "DebugChunkLog.hpp"
#include "FormatSpecs.hpp"
#include "NothrowLookup.hpp"

#include "ChunkConstants.hpp"
#include "ChunkHelpers.hpp"
#include "CoordTypes.hpp"
#include "ChunkMap.hpp"
#include "CommonUtils.hpp"
#include "Logger.hpp"
#include <memory>
#include <optional>
#include "cpp23_ranges.hpp"

using namespace glm;

const AABB*   ChunkMap::getBoundingBox(WorldChunkCoord chunkCoord) const{
    return &AT(entries,chunkCoord)->bounding_box;
}
