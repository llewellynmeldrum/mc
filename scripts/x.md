
Broken:

glVertexAttribPointer (error on params)
```xml
<command>
    <proto>void <name>glVertexAttribPointer</name></proto>
    <param><ptype>GLuint</ptype> <name>index</name></param>
    <param><ptype>GLint</ptype> <name>size</name></param>
    <param group="VertexAttribPointerType"><ptype>GLenum</ptype> <name>type</name></param>
    <param><ptype>GLboolean</ptype> <name>normalized</name></param>
    <param><ptype>GLsizei</ptype> <name>stride</name></param>
    <param>const void *<name>pointer</name></param>
</command>
```

Working:
glUniformMatrix4fv
```xml
<command>
    <proto>void <name>glUniformMatrix4fv</name></proto>
    <param><ptype>GLint</ptype> <name>location</name></param>
    <param><ptype>GLsizei</ptype> <name>count</name></param>
    <param><ptype>GLboolean</ptype> <name>transpose</name></param>
    <param kind="Matrix4x4" len="count*16">const <ptype>GLfloat</ptype> *<name>value</name></param>
</command>
```
