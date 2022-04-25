# CPU 3D Rasterizer/Renderer

To better understand how computer graphics work I decided to build my own _software_ renderer using rasterization as a hobby project.

**3D Rendering** is the process of generating a digital 3-dimensional image, often for use in games or movies. One common way of doing this is through rasterization. **A rasterizer** is a program that takes, as an input, an image described as triangle vertices and converts them into a _raster image_, i.e the corresponding pixels that the triangle would cover on a computer display. 

## Details
The renderer supports customizeable Vertex & Fragment shaders and processes an input list of vertices before passing them to the rasterizer. The rasterizer I have written uses a block based for loop to scan over the screen in 8x8 blocks and can quickly discard areas of the screen without any polygons. It determines if a pixel is covered by a triangle using edge equations. See section "The Edge Function" [here](https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage). It also features a camera matrix and can do view frustum culling using the [Sutherland-Hodgman Algorithm](https://en.wikipedia.org/wiki/Sutherland%E2%80%93Hodgman_algorithm).

While developing I mostly followed Trenki's excellent blog post on [developing a software renderer](https://trenki2.github.io/blog/2017/06/06/developing-a-software-renderer-part1/). If you also want to build your own I highly recommend reading it!

## Installation
TBC


## Pictures
TBC
