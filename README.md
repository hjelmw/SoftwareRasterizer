# Software Rasterizer/Renderer

To better understand the nitty gritty details of the [Graphics Pipeline](https://en.wikipedia.org/wiki/Graphics_pipeline) I decided to build my own _software_ renderer/rasterizer as a hobby project.

**Rasterization** is simply a rendering method used to map an image described in vector graphics to a raster image. **A rasterizer** is then a program that takes, as an input, an image described as triangle vertices and converts them into the corresponding pixels that the triangle would cover on a computer display. Rasterization is one of the main stages of the Graphics Pipeine


While developing I mostly followed Trenki's excellent blog post on [developing a software renderer](https://trenki2.github.io/blog/2017/06/06/developing-a-software-renderer-part1/) as well as JavidX9's Youtube series [Code-It-Yourself! 3D Graphics Engine](https://www.youtube.com/watch?v=ih20l3pJoeU)


## Details
The code is written entirely in C++ and my application can load vertex arrays described as .obj files, parse them and rasterize them to an output surface. The renderer additionally supports some nice to have features such as user programmable vertex & fragment shaders as well as camera controls.
