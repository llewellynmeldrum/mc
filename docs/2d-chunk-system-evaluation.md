# Evaluation: Switching to a 2D (Column) Chunk System

> Decision-support document (evaluation + recommendation). A concrete file-by-file
> migration plan is intentionally out of scope here.

## Context

The project (`mc`, a C++/OpenGL 4.1 voxel engine) currently uses **3D cubic chunks**
and wants to (a) simplify systems and (b) make Distant-Horizons-style quadtree LOD
tractable (an explicit "planned optimization", `README.md:34`). The user has confirmed
the world does **not** need infinite vertical extent — a **bounded Y axis is acceptable**.
That constraint is the key enabler: column chunks only make sense when world height is finite.

The question: is it worth migrating from 3D cubic chunks to 2D columns (full-height
chunks keyed by `(x, z)`)?

## How the current system actually works (confirmed in code)

- **Chunks are 16×16×16 cubes.** `CHUNK_XWIDTH=CHUNK_HEIGHT=CHUNK_ZWIDTH=16`
  (`include/ChunkInvariants.hpp:5-8`); `Chunk::data` is `std::array<Block, 4096>`
  (`include/Chunk.hpp:59`).
- **Chunks are keyed by a 3D coord.** `WorldChunkCoord = Coord3<...,i32>` with x/y/z
  (`include/CoordTypes.hpp:82`); the live map is `HashMap<WorldChunkCoord, ChunkEntry>`
  (`include/ChunkMap.hpp:50`).
- **Streaming is a 3D cube.** Generation/mesh candidates are spiral-iterated over
  `±SIMULATION_DIST` in **all three axes** (`src/Engine.cpp:161-193`, `224-253`);
  `World::chunksStatesInRadius` reserves `(2*dist+1)^3` (`include/World.hpp:34`).
  → Chunk count scales **cubically** with render distance. This is almost certainly why
  the repo notes "major issues on render distance > 8" (commit `3054c42`): at dist 8 a
  cube is 17³ ≈ 4913 chunks vs a column field of 17² ≈ 289.
- **No world-height bound exists today.** There is no `WORLD_HEIGHT` constant. Terrain
  noise sits ~y140–190 (`SEA_LEVEL=140`, `TERRAIN_HEIGHT()=156`, `MAX_ELEVATION_DELTA=32`,
  caves to ~y-150; `include/ChunkConcurrency.hpp:19-27`). So today the engine streams and
  meshes **empty air chunks above** and **solid stone chunks below** the terrain band —
  pure waste that a bounded column system removes.
- **Generation is already heightmap-driven (2D).** `HeightMap` samples 2D Perlin per
  `(x,z)` column, then *clamps to the current 3D chunk's Y bounds*
  (`src/ChunkGenerator.cpp:55-106`). For a vertical stack of chunks at one `(x,z)` the same
  2D noise is recomputed redundantly per chunk. A column generates it **once**.
- **The author already hand-clamps the vertical axis.** `RENDER_EXTENTS()` returns
  `{RENDER_DIST, 4, RENDER_DIST}` = `{6, 4, 6}` (`include/Engine.hpp`) — horizontal mesh
  radius is 6 but vertical is fixed at 4. This asymmetry is a tell: the cubic model is
  already being fought with a magic vertical cap. A column system makes that principled.
- **The renderer already has a per-chunk distance hook for LOD.** Meshes are stored
  per-chunk-coord in slot_maps (opaque + transparent) and sorted by squared distance to
  camera every frame (`src/Renderer.cpp:184-206`). Distance tiers for LOD selection slot
  in here naturally; no LOD/quadtree/octree code exists yet (only the `README.md:34` plan).
- **Stack:** C++23, CMake (≥3.28), OpenGL 4.1 + GLFW, FastNoiseLite, glm, ImGui.
- **Meshing uses 6 directional neighbours** (`getSurroundingBlocks`,
  `src/ChunkMesher.cpp:34-62`; `MeshJob.surroundingChunks` size `N_NEIGHBOURS=6`,
  `include/ChunkConcurrency.hpp:57`; `ChunkEntry.neighbours` is a 6-vector,
  `include/ChunkEntry.hpp:84`). Vertical (up/down) neighbours are two of those six.

## Pros of switching to 2D columns

1. **Kills the cubic scaling** that breaks render distance > 8. Loaded-set, hashmap size,
   and per-frame candidate iteration drop from O(d³) to O(d²). This is the single biggest win.
2. **Quadtree LOD becomes natural.** Distant Horizons-style LOD is fundamentally a 2D
   problem (a quadtree over the `(x,z)` ground plane, each node holding a height/colour
   sample). With columns, the LOD tree maps 1:1 onto the chunk grid; with cubic chunks
   you'd need an octree and cross-axis stitching that doesn't match how terrain is shaped.
3. **No more wasted air/stone chunks.** Bounded Y means each column covers exactly the
   playable band once — no streaming empty sky or deep stone purely because the camera's
   `y±dist` cube reaches them.
4. **Simpler neighbour model.** Vertical neighbours disappear (they're intra-column now),
   so meshing/face-culling needs only **4 horizontal neighbours** instead of 6. Removes a
   whole class of "vertical neighbour not yet generated → remesh" churn.
5. **Generation does its 2D work once per column**, not once per vertical slice — removes
   the redundant per-stack heightmap/noise recompute.
6. **Cheaper bookkeeping per visible area.** One `ChunkEntry`, one mesh-revision lifecycle,
   one AABB per column footprint instead of a vertical stack of them.

## Cons / costs of switching

1. **Larger, lumpier allocations & meshing units.** A full-height column (e.g. 16×256×16 ≈
   65k blocks) is one storage block and, naively, one mesh job — coarser granularity than
   16³. Per-frame remesh of a deep column (after a single block edit) is more work than
   remeshing one 16³ cube.
2. **Frustum culling gets coarser.** A tall column is rarely fully inside the frustum, so a
   column-granular cull keeps geometry the camera can't see. The current code already half-
   admits this by capping vertical extent at 4 — so in practice today's culling is already
   coarse vertically.
3. **Broad, invasive refactor.** The 3D `WorldChunkCoord` is threaded through coord
   conversions (`CoordTypes.hpp`), the three hashmaps (`ChunkMap`), streaming
   (`Engine.cpp`), `ChunkEntry`, neighbour wiring, the mesher, and the debug renderer.
   Collapsing the key to `(x,z)` touches all of them — structural, not local.
4. **Loses the cheap "skip a fully-air/fully-solid cube" sparsity.** Today an all-air 16³
   chunk is trivially skipped (`Chunk::isAllAir`, `include/Chunk.hpp:66`); a monolithic
   column must walk its whole height.
5. **Migration risk to the concurrency pipeline**, described in the README/commits as a
   hard-won "~1 month concurrency nightmare". Gen/mesh queues hash on the 3D `chunkCoord`
   (`STD_HASH_SPECIALIZATION`, `include/ChunkConcurrency.hpp:99-110`); re-keying touches the
   threaded path, which is the riskiest area to change.

Note: cons 1, 2, and 4 are exactly what optional internal **subchunks** would later
mitigate (see below) — they are deferrable, not blockers.

## Recommendation

**Yes — switch to full 2D `(x,z)` columns with a bounded `WORLD_HEIGHT`, and do NOT add
subchunks yet.** Your instinct is right: at this stage the simplicity win is the whole
point, and a second granularity system bought before culling actually needs it is
complexity without payoff. The big prizes — killing cubic scaling (the render-distance-8
wall), a natural quadtree LOD over the ground plane, deleting wasted air/stone chunks, and
4-neighbour meshing — all come from the *columnar keying alone*, not from subchunks.

This also matches what the code is already drifting toward: the vertical render extent is
hand-capped at 4 (`RENDER_EXTENTS = {6,4,6}`), generation is already 2D-heightmap-driven,
and meshing already slices neighbours into 2D boundaries. A column system makes the
existing hacks principled instead of adding a new concept.

### Answering the "two systems" worry directly

Your concern — how do you stop column-vs-subchunk confusion — is the deciding reason to do
columns *first* and treat subchunks as a strictly-later, strictly-internal detail. The rule
that prevents confusion:

- **There is exactly one addressable unit: the column.** One coordinate type
  (`WorldChunkCoord` becomes a 2D `(x,z)` key — or a new `ColumnCoord`), one hashmap key,
  one `ChunkEntry` per column, one gen job, one neighbour graph (4 horizontal links). The
  word "chunk" = "column", everywhere, no exceptions.
- **A subchunk is never a key, coordinate, map entry, job, or neighbour.** If/when it
  arrives, it is *only* an array index into a column's block storage and an offset into a
  mesh — an implementation detail of meshing/culling that the rest of the engine cannot
  name or look up. Nothing outside the mesher/culler is allowed to hold a "subchunk
  handle". That single constraint is what keeps it from becoming a parallel addressing
  system.
- **Sequence:** ship columns as the default and only unit now. Add subchunks later *only
  when a profiler shows* frustum/occlusion culling or per-edit remesh cost demands finer
  granularity — at which point they slot in behind the column's interface without a second
  coordinate space.

### Parameter to pick when implementing (not now)

`WORLD_HEIGHT` — the fixed column height in blocks (must be a multiple of 16 so a future
subchunk split is clean). The current terrain band (caves ~y-150 up to peaks ~y190) implies
roughly 256–384 is comfortable; 256 (=16 subchunks) is the natural default.

## Bottom line

Switch to full 2D columns. It simplifies the engine, removes the cubic-scaling wall that's
already biting at render distance > 8, and turns the planned Distant-Horizons LOD from an
octree-stitching problem into a clean quadtree over the ground plane. Keep subchunks as a
named-but-deferred internal optimization, governed by the one-addressable-unit rule above,
so the two never compete.
