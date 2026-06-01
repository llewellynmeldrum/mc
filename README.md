# minecraft clone, opengl4.1 and glfw
<img width="800" alt="image" src="https://github.com/user-attachments/assets/f83296e1-f64c-497b-8004-4b817cf4b9aa" />

# Current features:
- (mostly) correct back-to-front transparency rendering
- Concurrent meshing and generation pipelines
- Debug rendering mode to see which state a chunk is in (i.e meshing, generated, in certain queue, etc)
- [ImGui](https://github.com/ocornut/imgui) debug hud with some decently detailed perf stats: <br>
<img width="350" alt="image" src="https://github.com/user-attachments/assets/0c8265c5-a056-4530-bc9c-9c56f5148eff" />
<br>
- glorious homemade textures (my favourite so far is grass)<br>
- <img width="200" alt="image" src="https://github.com/user-attachments/assets/8cb6a114-433f-4ed8-bcfe-3d8c9f1c476f" />
<br>
- Shitty caves, trees, and hills/lakes. 

## optimizations (implemented)
- Faces touching opaque faces are culled from meshes (chunk generation makes surrounding chunks trigger remesh, so this applies to almost all cases where it can, including on chunk boundaries)
- Translucent faces of the same material which touch each other are culled
- World generation and chunk meshing happens off main thread
- Chunks are culled ocasionally if they become `render_distance+2` far away from the camera.
- All the opengl backface culling and whatnot is enabled (where possible)
- EBO index buffer for slightly less vertex data per face
  
## optimizations (planned)
- Frustum culling (mostly done, but my implementation has some bugs probably in the math somewhere.)
- !!!! Oclussion culling
- Chunk neighbours uploaded to meshJobs should only get single slice of neighbours blocks
- Pack vertex data, could massively save on gpu memory
- Implement instanced renderer (could do the entire scene in a couple draw calls)
- chunk mesh decimation/ LOD at further distances (like Distant Horizons mod in minecraft)
- greedy meshing coplanar faces of same textured faces
- switch mesh sorting algorithm to a partial sort with a 'budget'


# Progress:
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


# Tools used
- gnu make+ Cmake
- nvim (btw)
- compiledb
- GIMP

# Libs used
- glfw (for windowing and input)
- glm (opengl math library, for vector types and matrix math)
- glbinding (loads opengl functions with c++11 bindings, slightly stronger typing than GLAD)
- FastNoise2 (noise functions for terrain generation)

# Resources used
- [learnopengl.com](https://learnopengl.com/Introduction) - excellent resource for learning opengl for basic 3D, especially with backwards compatability in mind (macos users rejoice)

- [An Analysis of Minecraft-like Engines - mikolalysenko](https://0fps.net/2012/01/14/an-analysis-of-minecraft-like-engines/) - Great (kinda math heavy) article about some chunk optimisations i hope to use

- [Meshing in a Minecraft Game - mikolalysenko](https://0fps.net/2012/06/30/meshing-in-a-minecraft-game/) - Another cool article
- [The World Generation Of Minecraft - Alan Zucconi](https://www.alanzucconi.com/2022/06/05/minecraft-world-generation/) - good intro to minecrafts world gen
