cmake_minimum_required(VERSION 3.17)
project(Raytracer)

set(CMAKE_CXX_STANDARD 20)

find_package(glm CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(OpenMP)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
    include_directories("/usr/local/include" "/usr/local/opt/llvm/include")
    link_directories("/usr/local/lib" "/usr/local/opt/llvm/lib")
endif()

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
        )

set(OBJLOAD
        lib/OBJ-Loader/Source/OBJ_Loader.h
)

# load paths to resource files as definitions
set(RESOURCE_FILES )
set(RESOURCE_FULL_PATHS )
FILE(GLOB RESOURCE_FULL_PATHS resources/*.obj)
foreach(fullname ${RESOURCE_FULL_PATHS})
    get_filename_component(filename ${fullname} NAME)
    list(APPEND RESOURCE_FILES ${filename})
endforeach()
include(LoadResourceFiles.cmake)
message(${RESOURCES_DEBUG_COMPILE_DEFINITIONS})


set(COMPILE_DEFINITIONS ${RESOURCES_DEBUG_COMPILE_DEFINITIONS})

add_executable(${PROJECT_NAME} ${IMGUI} ${OBJLOAD}
    main.cpp
    Raytracer.cpp
    Raytracer.h
    AppSettings.h
    AppSettings.cpp
    Scenes.h

    materials/Material.h
    materials/Metal.h
    materials/Dielectric.h
    materials/BSSRDF.h
    materials/Lambertian.h

    common/ColorBuffer.h
    common/Random.h
    common/Camera.cpp
    common/Camera.h
    common/Intersection.h
    common/Ray.h
    common/MathUtils.h
    common/ScatterInfo.h
    common/Pdf.h

    GLviewer/GLViewer.cpp
    GLviewer/GLViewer.h
    GLviewer/RenderStatistics.cpp
    GLviewer/RenderStatistics.h
    GLviewer/Utils.h

    scene/SceneMgr.cpp
    scene/SceneMgr.h
    scene/SceneObject.h
    scene/SceneObject.cpp
        scene/objects/Sphere.cpp
        scene/objects/Plane.cpp
        scene/objects/Plane.h
        scene/objects/Sphere.h
        scene/objects/Polygon.cpp
        scene/objects/Polygon.h
        scene/objects/ObjLoader.h
    scene/AccelerationDS.h
        scene/accelerationDS/LinearADS.h
        scene/accelerationDS/LinearADS.cpp
        scene/accelerationDS/AABBSimpleADS.h
        scene/accelerationDS/AABBSimpleADS.cpp
        scene/accelerationDS/AABBBVH.h
        scene/accelerationDS/AABBBVH.cpp
    scene/AABB.h
    scene/AABB.cpp
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_link_libraries(${PROJECT_NAME} PUBLIC glm SDL2::SDL2-static GLEW::glew)
else()
    target_link_libraries(${PROJECT_NAME} PUBLIC glm::glm SDL2::SDL2-static GLEW::glew OpenMP::OpenMP_CXX)
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC ${COMPILE_DEFINITIONS})

add_executable(OBJ-example ${OBJLOAD} lib/OBJ-Loader/examples/1\ -\ LoadAndPrint/e1_loadandprint.cpp)
