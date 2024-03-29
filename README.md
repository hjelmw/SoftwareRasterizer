# Real Time Software Rasterizer/Renderer

![alt text](https://github.com/hjelmw/SoftwareRasterizer/blob/master/img/combined.png)

To better understand the nitty gritty details of the [Graphics Pipeline](https://en.wikipedia.org/wiki/Graphics_pipeline) I decided to build my own _software_ renderer/rasterizer as a hobby project.

**Rasterization** is simply a rendering method used to map an image described in vector graphics to a raster image. **A rasterizer** is then a program that takes, as an input, an image described as triangle vertices and converts them into the corresponding pixels that the triangle would cover on a computer display. Rasterization is one of the main stages of the Graphics Pipeine.

## Details
The application is written in C++ and apart from maybe window management with SDL2 **runs entirely on the CPU**. The application can load vertex arrays described as .obj files, parse them and rasterize them to an output surface. It has many standard rendering features such as backfacing and view-frustum culling/clipping and there is also a hierarchical Z-buffer to ensure drawing triangles in the correct order. 

The renderer additionally supports some nice to have features such as arbitrary per vertex attributes and interpolation, user programmable vertex & fragment shaders as well as camera controls.


While developing I used 3 main resources: Trenki's excellent blog post on [Developing a Software Renderer](https://trenki2.github.io/blog/2017/06/06/developing-a-software-renderer-part1/), JavidX9's Youtube series [Code-It-Yourself! 3D Graphics Engine](https://www.youtube.com/watch?v=ih20l3pJoeU), and many chapters of the online book [Scratchapixel](https://www.scratchapixel.com/index.php?redirect). When writing the code, I tried to closely follow the concepts of the graphics pipeline so that the application would behave in a similar manner to a GPU accelerated Graphics API.

To see how it can be used, have a look in the [Main.cpp](https://github.com/hjelmw/SoftwareRasterizer/blob/master/src/Main.cpp) file! The entrypoint for rasterization is in `Rasterizer::drawTriangles` found [here](https://github.com/hjelmw/SoftwareRasterizer/blob/a839aaed5694e1d921fe6d82629a532b7768a0c8/src/Rasterizer.cpp#L429)


## Setup
**Run Rasterizer.exe** in the `samples/` folder

Or build it locally
1. Download the [Simple DirectMedia Layer SDK](https://www.libsdl.org/index.php) 
2. Open Cmake in the root directory
3. Configure and point it to SDL lib and include folder
4. Copy SDL2.dll to where you will build the binaries, e.g `SoftwareRasterizer/build`
5. Additionally place your .obj file inside the same build folder
6. Correct the file path in `Rasterizer::loadModelIntoVertexArray()`
7. Compile and run! 

`WASD` as well as `RF` translates the camera. Additionally by holding down the right mouse button you can orient yourself in the scene
