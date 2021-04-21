//
// Created by Jan Sobol on 26/11/2020.
//

#include "GLViewer.h"
#include "../Scenes.h"
#include "Utils.h"
#include <iostream>

#include "../lib/imgui/imgui.h"
#include "../lib/imgui/imgui_impl_sdl.h"
#include "../lib/imgui/imgui_impl_opengl3.h"

GLViewer::GLViewer() : backgroundColor(AppSettings::backgroundColor) {
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(
        "ogl",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        AppSettings::imgWidth,
        AppSettings::imgHeight,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
    );

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,SDL_GL_CONTEXT_DEBUG_FLAG);
    context = SDL_GL_CreateContext(window);

    std::cout
            << "Vendor   : " << glGetString(GL_VENDOR) << std::endl
            << "Renderer : " << glGetString(GL_RENDERER) << std::endl
            << "Version  : " << glGetString(GL_VERSION) << std::endl
            << "GLSL     : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    //std::cout << "Extensions :\n" << glGetString(GL_EXTENSIONS) << std::endl;

    glewInit();

    initRaytracer();
    initGLObjects();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init("#version 410 core");
}

void GLViewer::run() {

    glClearColor(0,1,0,1);

    SDL_Event event;
    while(running){
        while(SDL_PollEvent(&event)){
            ImGui_ImplSDL2_ProcessEvent(&event);
            if(event.type == SDL_QUIT){
                running = false;
            } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
                glViewport(0, 0, event.window.data1, event.window.data2);
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        updateBuffer();

        ImGui::Begin("Raytracer");
        ImGui::Text("Current samples per pixel = %d", renderedSamples);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        if (ImGui::Checkbox("Antialiasing", &AppSettings::antialiasing)) needReset = true;
        if (ImGui::InputFloat("Distance to focus", &AppSettings::distToFocus, 1.f)) { raytracer->initCameraWithAppSettings(); needReset = true; }
        if (ImGui::InputFloat("Aperture", &AppSettings::aperture, 0.1f)) { raytracer->initCameraWithAppSettings(); needReset = true; }
        bool ambient = AppSettings::backgroundColor != glm::vec3 (0.f);
        if (ImGui::Checkbox("Ambient light", &ambient)) { AppSettings::backgroundColor = ambient ? backgroundColor : glm::vec3 (0.f); needReset = true; }
        ImGui::InputText("", filename, 30); ImGui::SameLine();
        if (ImGui::Button("Save")) { needSaveToFile = true; }
        if (ImGui::Checkbox("Use Monte Carlo", &AppSettings::useMC)) needReset = true;
        if (ImGui::Checkbox("Enable lights direct sampling", &AppSettings::lightsDirectSampling)) needReset = true;
        if (ImGui::Button("Next scene")) {
            selectedScene = (selectedScene + 1) % (scenes.size());
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset raytracing")) {
            needReset = true;
        }
        ImGui::End();

        scenes[selectedScene]->gui(needReset);

        ImGui::Render();

        //frame
        glClearColor(0,1,0,1);
        glClear(GL_COLOR_BUFFER_BIT);

        draw();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void GLViewer::initRaytracer() {
    raytracer = std::make_shared<Raytracer>();
    colBuff = std::make_shared<ColorBuffer>(AppSettings::imgWidth, AppSettings::imgHeight);

    initScenes();
    initSelectedScene();

    workingThread = std::make_unique<std::thread>([this] { render(); });
}

void GLViewer::render() {
    while (true) {
        unsigned _selectedScene = selectedScene;

        raytracer->renderStage(*colBuff, AppSettings::imgWidth, AppSettings::imgHeight);
        renderedSamples++;

        if (_selectedScene != selectedScene) {
            initSelectedScene();
            needReset = true;
        }

        if (needReset) {
            colBuff->clear();
            renderedSamples = 0;
            needReset = false;
        }

        if (needSaveToFile) {
            colBuff->saveToPPM(
                    filename,
                    [this](const glm::vec3 &pixel) { return this->raytracer->pixelColorOperation(pixel, this->renderedSamples); }
                    );
            needSaveToFile = false;
        }

        renderStageCompleted = true;

        if (!running) break;

        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk);
        lk.unlock();
    }
}

void GLViewer::initGLObjects() {
    programId = createProgram(
            createShader(GL_VERTEX_SHADER, vsSrc),
            createShader(GL_FRAGMENT_SHADER, fsSrc)
            );

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    texUniform = glGetUniformLocation(programId, "tex");

    glGenVertexArrays(1, &vao);
}

void GLViewer::initScenes() {
    //scenes.emplace_back(std::make_shared<SimpleScene>());
    // scenes.emplace_back(std::make_shared<CornellBox>());
    // scenes.emplace_back(std::make_shared<CornellBox2>());
    // scenes.emplace_back(std::make_shared<MaterialScene>());
    // scenes.emplace_back(std::make_shared<BlenderTest>());
    scenes.emplace_back(std::make_shared<LightedCube>());
    scenes.emplace_back(std::make_shared<ObjTest>());
}

void GLViewer::updateBuffer() {
    if (renderStageCompleted) {

        glBindTexture(GL_TEXTURE_2D, textureId);
        auto data = pixelsToTextureData();
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, colBuff->width, colBuff->height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     data.data());

        renderStageCompleted = false;
        std::lock_guard<std::mutex> lk(m);
        cv.notify_one();
    }
}

void GLViewer::draw() {
    glUseProgram(programId);

    // Texture 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(texUniform, 0);

    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

std::vector<unsigned char> GLViewer::pixelsToTextureData() const {
    std::vector<unsigned char> buffer(colBuff->size() * 3);
    auto width = colBuff->width;
    auto height = colBuff->height;
    for (int j =  height - 1; j >= 0 ; j--)
    {
        for (int i = 0; i < width; ++i)
        {
            auto pixel = raytracer->pixelColorOperation(colBuff->p[j*width + i], renderedSamples);
            buffer[j*width*3 + i*3 + 0] = int(255.99 * pixel.r);
            buffer[j*width*3 + i*3 + 1] = int(255.99 * pixel.g);
            buffer[j*width*3 + i*3 + 2] = int(255.99 * pixel.b);
        }
    }
    return buffer;
}

GLViewer::~GLViewer() {
    workingThread->join();
}

void GLViewer::initSelectedScene() {
    raytracer->clearScene();
    scenes[selectedScene]->createScene(raytracer->scene());
}
