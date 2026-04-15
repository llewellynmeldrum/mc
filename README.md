# mcpp - a minecraft cpp clone


## Usage of autodoc generator for GLAD
1. install `python3` of some kind
2. source the `.venv` if you want, no deps at the moment
3. make sure the `gl.xml` file exists in `./resources`
```bash
# if not:
mkdir ./resources
wget https://registry.khronos.org/OpenGL/xml/gl.xml -O resources/gl.xml
```
4. run the script: `python3 ./scripts/annotate_glad_header.py`

It will generate a `annotated_gl.h` file (inside `./include`), which you just need to include after `glad/gl.h`.
Example output
```cpp
//  @param n
//  @param programs, len: n
//  @param residences, len: n
	inline GLboolean glAreProgramsResidentNV(GLsizei n, const GLuint *programs, GLboolean *residences){
		return     glad_debug_glAreProgramsResidentNV(n, programs, residences);
	}
```

I havent actually tested if there is a runtime overhead cost of this. I should probably force them all to be inline via attributes to minimize that, but we will cross that bridge when we get to it.

# Tools used
gnu make
nvim
compiledb (bear broke for some reason)
