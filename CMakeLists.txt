cmake_minimum_required(VERSION 3.17)
project(Raytracer)

set(CMAKE_CXX_STANDARD 20)

find_package(glm CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)

set(IMGUI
        lib/imgui/imconfig.h
        lib/imgui/imgui.cpp
        lib/imgui/imgui.h
        lib/imgui/imgui_demo.cpp
        lib/imgui/imgui_draw.cpp
        lib/imgui/imgui_internal.h
        lib/imgui/imgui_widgets.cpp
        lib/imgui/imstb_rectpack.h
        lib/imgui/imstb_textedit.h
        lib/imgui/imstb_truetype.h
        lib/imgui/imgui_impl_opengl3.cpp
        lib/imgui/imgui_impl_opengl3.h
        lib/imgui/imgui_impl_sdl.cpp
        lib/imgui/imgui_impl_sdl.h
        materials/Lambertian.h common/MathUtils.h)

add_executable(${PROJECT_NAME} ${IMGUI} main.cpp Raytracer.cpp Raytracer.h common/Ray.h scene/SceneMgr.cpp scene/SceneMgr.h scene/SceneObject.h common/Intersection.h AppSettings.h materials/Material.h common/ColorBuffer.h common/Random.h common/Camera.cpp common/Camera.h AppSettings.cpp scene/objects/Sphere.cpp scene/objects/Sphere.h scene/objects/Polygon.cpp scene/objects/Polygon.h materials/Metal.h GLviewer/GLViewer.cpp GLviewer/GLViewer.h GLviewer/Utils.h Scenes.h materials/Dielectric.h)

target_link_libraries(${PROJECT_NAME} PUBLIC glm SDL2::SDL2-static GLEW::glew)