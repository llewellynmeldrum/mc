# minecraft clone, opengl4.1 and glfw

# Progress:
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


# Tools used
- gnu make
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
