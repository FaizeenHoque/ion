I started building `ion` because I was tired of using other people's engines
without understanding what was actually happening underneath them. It's a
from-scratch attempt to understand the OpenGL rendering pipeline and
immediate-mode UI by building a real one instead of reading about it: a
small 3D scene editor written in C++, with a dockable interface for moving
objects and lights around a live viewport. The longer-term goal is to use
it as a base for running and visualizing my own simulations, though that
part hasn't been built yet. Right now it's a scene editor, not a
simulation tool.

## What ion does

There's a scene graph holding objects (quad, cube, sphere, and monkey
primitives so far) and lights, and a docked editor UI built on Dear ImGui
with Hierarchy, Properties, Viewport, and Explorer panels. Selecting an
object in the Hierarchy panel lets you edit its transform, scale, and color
live in the Properties panel, and the same works for lights and their
direction and color.

Camera movement follows the same conventions as Unity's scene view: hold
right mouse over the viewport to look around and move with WASD, hold
middle mouse to pan, and scroll to dolly forward or back. Holding shift
while flying triples the movement speed.

## How the viewport works

The scene doesn't render straight to the window. Instead, `ion` renders
into an offscreen framebuffer every frame, and that framebuffer's texture
gets displayed inside the ImGui Viewport panel. That's what lets the
viewport behave like any other dockable panel: resizable, movable, laid
out however you want, instead of being locked to the shape of the window.
Whenever the panel is resized, the framebuffer is rebuilt to match, so the
rendered image always matches the space it's shown in.

Lighting and environment currently come from a skybox and a single
directional light, and there's an infinite reference grid for orientation
in empty space.

## What's not built yet

The Explorer panel is currently just a placeholder. It doesn't do
anything yet. Nothing in the scene can be saved or loaded yet, so every session
starts from the same default scene. `FQRenderer`, meant to become a
post-processing pass, is an empty stub right now. All of this is actively
being worked on.

## What this is written with

The program is written in C++17. OpenGL 3.3 core profile is used for
rendering, GLFW creates the window and handles input, GLAD loads OpenGL
functions, Dear ImGui's docking branch provides the editor interface, GLM
handles vector and matrix math, and stb_image loads textures. All of these
are vendored under `libraries/`.

## How to install

You need CMake, a C++17 compiler, and OpenGL 3.3+ drivers. Clone the
repository along with its submodules, since the dependencies above are
pulled in from `libraries/` rather than installed system-wide:

```bash
git clone --recurse-submodules https://github.com/FaizeenHoque/ion
cd ion
```

Then configure and build the project from its root directory:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

The full dependency wiring, including how each library is linked, is in
`CMakeLists.txt`.

## Screenshots

<img width="1800" height="950" alt="image" src="https://github.com/user-attachments/assets/3ef89765-ecba-4560-a8a4-892d3fbeefe2" />
<img width="363" height="268" alt="image" src="https://github.com/user-attachments/assets/72112cc6-9341-42a2-9ad3-d2506b04b13f" />

## More is coming

This is still an early-stage project. Scene saving and loading, more
primitive types, a working Explorer panel, and the post-processing pass are
all planned next.
