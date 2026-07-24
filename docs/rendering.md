# Light level design

Similar to minecraft, there will be two 'light levels', howeverone will be component-style rgb, and the other will simply be a single value. 


Two types of light:

## Stellar Light
- Propogates from world height through -Y
- Per vertex, we store 4 bits 'intensity' - this intensity is used as a scalar for the u_sunlight_color, which is updated whenever the daytime changes.
- We can create a remap table of colors which corresponds to a certain time of day. to begin with for now we will just set it as one.

### Shader variables: 
```glsl
uniform vec4 u_sunlight_color;

```

## Block Light
- Propogates from blocks which emit light, a part of their block definition
- Per vertex, we store 4 bits red, 4 bits green, 4 bits blue. 
A vertex can determine its light color entirely from the incoming block color.

We can have a light field on the vertex which can be a 16 bit (2 byte)




|Name| Bit width|
|-|-|
|sunlight_intensity|4|
|blocklight_r|4|
|blocklight_g|4|
|blocklight_b|4|


|Name | Current storage (bits)| required storage (bits) | required range| wasted bits | 
|-|-|-|-|-|
|face_direction|32|3|(0,5)| 29|
|faceOpacity |32|?|could be anything really| ?|
|texture_atlas_id|32|?|3-4 bits is pretty safe| ?|


Lets say we had 3 bits face_dir, 3 bits texture_atlas_id, 8 bits faceOpacity? 2 bit leftover.

And then face opacity could just be a number between 0,255. 


|Name| Bit width|
|-|-|
|sunlight_intensity|4|
|blocklight_r|4|
|blocklight_g|4|
|blocklight_b|4|
|face_dir|3|
|block_tex_id|3|
|block_tex_id|8|
|N/A|2|


```c++
// we dont actually use bitfields, this is for demonstration purposes
struct packed_attr{
	uint sunlight_intensity : 4;
	uint blocklight_r       : 4;
	uint blocklight_g       : 4;
	uint blocklight_b       : 4;
	uint face_dir           : 3;
	uint block_tex_id       : 3;
	uint block_tex_id       : 8;
};

```


The number of light levels is subject to change, but I think 15 is a good amount. (2^4, meaning we can store it in 4 bits each.
