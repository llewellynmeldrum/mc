# minecraft clone, opengl4.1 and glfw
<img width="900" alt="image" src="https://github.com/user-attachments/assets/80ba4c24-55ee-4903-8f14-8c056f924581" />
<img width="900" alt="image" src="https://github.com/user-attachments/assets/92b01154-5142-4928-85cd-dd3ac803bd62" />

# Current features:
- Infinite procedural terrain generation
- Trees, foliage, and blocks all dependent on biome, which is also procedural and 100% deterministic to world seed. (The same world seed will always produce the same layout of biomes, blocks, features, etc, regardless of chunk generation order) 
- Correct transparency (& cutout) rendering
- Multithreaded chunk generation and meshing
- Separate debug rendering pipeline to display 3D visuals (chunk/frustum borders, visualize noise, etc)
- [ImGui](https://github.com/ocornut/imgui) debug hud with some decently detailed perf stats: <br>
<img width="350" alt="image" src="https://github.com/user-attachments/assets/0c8265c5-a056-4530-bc9c-9c56f5148eff" />
<br>
- All textures are made from scratch in GIMP. Heavily inspired by minecraft of course: <br>
<img width="200" alt="image" src="https://github.com/user-attachments/assets/38184e51-2b0a-4351-a377-c76921e8ebdb" />

<br>

## optimizations (implemented)
- frustum culling:<br>
<img width="640" height="348" alt="frustum2" src="https://github.com/user-attachments/assets/3e114e42-fbbf-46ac-9e62-8af1b6cad776" />

<br>

- Faces touching opaque faces are culled from meshes (chunk generation makes surrounding chunks trigger remesh, so this applies to almost all cases where it can, including on chunk boundaries)
- Translucent faces of the same material which touch each other are culled
- World generation and chunk meshing happens off main thread
- Chunks are culled ocasionally if they become `render_distance+2` far away from the camera.
- All the opengl backface culling and whatnot is enabled (where possible)
- EBO index buffer for slightly less vertex data per face
  
## optimizations (planned)
- !!!! Oclussion culling
- Pack vertex data, could massively save on gpu memory
- Implement instanced renderer (could do the entire scene in a couple draw calls) (update: did this for debug chunk renderer)
- chunk mesh decimation/ LOD at further distances (like Distant Horizons mod in minecraft)
- greedy meshing coplanar faces of same texture
- switch mesh sorting algorithm to a partial sort with a frame budget

# Progress:

> [!NOTE]
>  TODO: add commit links to each progress point

_This is my first time messing with 3d graphics, so I documented the process of following along with [learnopengl.com](https://learnopengl.com/Introduction)._

First cube, first textured cube:<br>
<img height="250" alt="spinny" src="https://github.com/user-attachments/assets/2b702fee-f9cd-4036-8f6c-9b2ae545e951" />
<img height="250" alt="Pasted image 20260420222220" src="https://github.com/user-attachments/assets/18a40435-0547-4b7b-94da-3199b133aafe" />

first chunk, no culling <br>
<img height="250" alt="Pasted image 20260422170131" src="https://github.com/user-attachments/assets/4aa77a7c-15f3-4734-b162-67555aba4811" />
<img height="250" alt="Pasted image 20260422170156" src="https://github.com/user-attachments/assets/c63c710d-5fe1-4025-a2e8-e02b485d649c" />

Got culling working, debug overlay done<br>
<img  height="250" alt="Pasted image 20260423183016" src="https://github.com/user-attachments/assets/e62f396b-491d-4163-994b-79145ff0dc27" />

Stress tested with 16x16 chunks, decent fps. Also nice graph and stats on debugui<br>
<img  height="250" alt="Pasted image 20260424170231" src="https://github.com/user-attachments/assets/bef6a484-581e-4518-af55-17e1409ff4f4" />

Updated texture<br>
<img height="400" alt="image" src="https://github.com/user-attachments/assets/471c78f8-0bf6-40cd-a192-e6b817391049" />

Began working on terrain gen<br>
<img width="500" alt="image" src="https://github.com/user-attachments/assets/f40c4002-ef50-41e0-bba1-101235e341a8" />
<br>
<img width="500" alt="image" src="https://github.com/user-attachments/assets/6bdb2aae-762c-4afb-a83d-4f35f510a54b" />

Implemented cave generation after a ~1 month concurrency nightmare
<img width="700" alt="Pasted image 20260528194646" src="https://github.com/user-attachments/assets/e8e08448-c3a9-4a53-b608-7f1ac8893579" />
<br>
<img width="700" alt="image" src="https://github.com/user-attachments/assets/53b66b90-2416-45fa-895d-d841188fbaa4" />
<br>
Added trees<br>
<img width="500" alt="image" src="https://github.com/user-attachments/assets/f83296e1-f64c-497b-8004-4b817cf4b9aa" />
<br>
Added frustum culling <br>
<img width="640" height="348" alt="frustum2" src="https://github.com/user-attachments/assets/3e114e42-fbbf-46ac-9e62-8af1b6cad776" />

Added heaps of new blocks, overhauled wordlgen, added more trees and foliage, now have 4 block shapes, 5 biomes, ~30 different blocks <br>
<img width="900" alt="image" src="https://github.com/user-attachments/assets/80ba4c24-55ee-4903-8f14-8c056f924581" />

# Tools used
- gnu make
- CMake (thanks chat gippity)
- lldb
- gcc16 / clang22
- GIMP
- nvim (btw)

# Libs used
- glfw (for windowing and input)
- glm (opengl math library, for vector types and matrix math)
- glbinding (loads opengl functions with c++11 bindings, slightly stronger typing than GLAD)
- ~FastNoise2~ FastNoiseLite (noise functions for terrain generation)
- cpptrace (for debugging)
- Tracy (for profiling)


# Resources used
- [learnopengl.com](https://learnopengl.com/Introduction) - excellent resource for learning opengl for basic 3D, especially with backwards compatability in mind (macos users rejoice)

- [An Analysis of Minecraft-like Engines - mikolalysenko](https://0fps.net/2012/01/14/an-analysis-of-minecraft-like-engines/) - Great (kinda math heavy) article about some chunk optimisations i hope to use

- [Meshing in a Minecraft Game - mikolalysenko](https://0fps.net/2012/06/30/meshing-in-a-minecraft-game/) - Another cool article
- [The World Generation Of Minecraft - Alan Zucconi](https://www.alanzucconi.com/2022/06/05/minecraft-world-generation/) - good intro to minecrafts world gen
- [www.demangler.com](https://www.demangler.com/) - holy goated 
- [vkguide multithreading blog](https://vkguide.dev/docs/extra-chapter/multithreading/#:~:text=The%20first%20and%20most%20classic,them%20perform%20their%20own%20task)
- [Ex mcpe dev's article on optimizing caves](https://tomcc.github.io/2014/08/31/visibility-2.html)
- [mcsrc.dev](https://mcsrc.dev) really cool tool for browsing minecraft source code legally. I am larping i barely understood 5% of the java slop
