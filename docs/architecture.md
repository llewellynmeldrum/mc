# Architecture

DebugBlock - contain

(in progress of updating)
# Rendering
## Rendering Glossary

| word |meaning|
|-|-|
|Depth Buffer|A per frag buffer which stores the relative (0.0,1.0) depth value/distance of an object from the viewer.|
|Depth Testing|A per frag operation which discards frags that are occluded by a nearer object in the depth buffer|
|Color blending|A per frag operation which combines incoming colors with existing colors in the color buffer |

## Passes:

*3 Passes, inspired by minecraft*
1. Opaque - Quads with NO blending, and NO transparent texels. solid blocks, minecraft calls it SOLID
2. Cutout - Quads with NO blending, but some 100% transparent texels. 
3. Blended - Quads WITH some blending,objects with some sort of partial transparency. Minecraft calls it TRANSLUCENT



## Opaque Layer
DEPTH MASKING: ON (we want our writes to go on the depth buffer)
DEPTH TESTING: ON (we dont want to overdraw, especially if nearest sorted)
COLOR BLENDING: OFF (we dont want to mix colors already on the color buffer) 

## Cutout Layer
DEPTH MASKING: ON (we want our writes to go on the depth buffer)
DEPTH TESTING: ON (we dont want to overdraw, especially if nearest sorted)
COLOR BLENDING: OFF (we dont want to mix colors already on the color buffer) 

## Blended Layer
DEPTH MASKING: OFF (We dont need the depth buffer)
DEPTH TESTING: OFF (we WANT to overdraw, to mix colors.)
COLOR BLENDING: ON (we WANT to mix colors already in the color buffer)

## Relevant gl functions:
| function |purpose|
|-|-|
|glDepthMask(bool)|Enables/disables writes to the depth buffer |
|glEnable(GL_DEPTH_TEST)|Enables/disables depth testing, i.e the early discard of occluded fragments|
|glEnable(GL_BLEND)|Enables/disables color blending, i.e the mixing of incoming colors with those already in the color buffer|
|glEnable(GL_CULL_FACE)|Enables/disables back-face culling (for the selected winding order)|

## Block Shapes
Block shapes will be an enum that is branched on by the mesher.
Most blocks are BlockShape::cube, some (like grass) are BlockShape::cross_cube
The mesher will branch on this.
TODO: It might also be nice to have BlockShape::cross blocks have a height param, in blocks, which dictates how tall their mesh is.

