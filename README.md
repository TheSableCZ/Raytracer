# RayTracer

GitHub: https://github.com/TheSableCZ/Raytracer

## Dependencies:

Included static libraries
- [Dear ImGui](https://github.com/ocornut/imgui)

  Source code of Dear ImGui is expected to be found in `lib/imgui`.

- [OBJ LOADER](https://github.com/Bly7/OBJ-Loader)

  Source code of Dear ImGui is expected to be found in `lib/OBJ-Loader`.

External dependencies:
- **OpenGL 4.1** and above
- **glm** https://github.com/g-truc/glm
- **SDL2** https://www.libsdl.org/
- **GLEW** http://glew.sourceforge.net/
- **OpenMP** (Optional)

Due to file limit on WIS VUT servers, scenes are not included in `resources` folder.
If the folder is empty please download scene files by hand from repository above.

Required files are:

- `bunny_scene.obj`
- `Room.obj`
- `dyno.obj`
- `cubeSceneDebug.obj`
- `untitled.obj`
- `untitled2.obj`

## Installation

This is a common cMake project:

Please note that is recommended to build in release mode for better performance.
