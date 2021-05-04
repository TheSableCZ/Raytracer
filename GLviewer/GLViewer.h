//
// Created by Jan Sobol on 26/11/2020.
//

#ifndef RAYTRACER_GLVIEWER_H
#define RAYTRACER_GLVIEWER_H

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "../Raytracer.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include "RenderStatistics.h"

class Scene;

class GLViewer {
public:
    GLViewer();
    ~GLViewer();
    void run();
    void initRaytracer();
    void initGLObjects();
    void initScenes();
    void initSelectedScene();
    void updateBuffer();
    void draw();

protected:
    std::vector<unsigned char> pixelsToTextureData() const;

private:
    SDL_Window *window;
    SDL_GLContext context;
    bool running = true;

    std::shared_ptr<Raytracer> raytracer;
    std::shared_ptr<ColorBuffer> colBuff;
    int renderedSamples = 0;
    bool needReset = false;
    bool needSaveToFile = false;
    bool neednewScene = false;

    GLuint programId;
    GLuint bufferId;
    GLuint textureId;
    GLuint vao;
    GLuint texUniform;

    void render();
    std::unique_ptr<std::thread> workingThread;
    std::condition_variable cv;
    std::mutex m;
    bool renderStageCompleted = false;

    std::vector<std::shared_ptr<Scene>> scenes;
    int selectedScene = AppSettings::defaultScene;
    int current_ac_technique = AppSettings::defaultAccelerationDS;
    glm::vec3 backgroundColor;
    char filename[30] = "image.ppm";

    RenderStatistics statistic;
    Measurement prepareTime;
    unsigned long actPrimitiveCount = 0;
};

std::string const vsSrc = R".(
  #version 410 core

  //out vec3 vColor;
  out vec2 vCoord;

  //uniform int width;
  //uniform int height;

  //layout(location=0)in vec3 color;

  void main(){
    //gl_Position = vec4(2*float(gl_VertexID % width) / float(width) - 1, 1 - 2*float(height - gl_VertexID / width) / float(height), 0, 1);
    //gl_Position = vec4(2*((gl_VertexID % width) / width) - 1, 2*gl_VertexID/width - 1, 0.f, 1.f);
    //vColor = color;
    vCoord = vec2(gl_VertexID&1, gl_VertexID>>1);
    vec2 pos = vec2(-1+2*(gl_VertexID&1),-1+2*(gl_VertexID>>1));
    gl_Position = vec4(pos,0,1);
  }

  ).";

std::string const fsSrc = R".(
  #version 410 core

  //in vec3 vColor;
  in vec2 vCoord;

  uniform sampler2D tex;

  layout(location=0)out vec4 fColor;

  void main(){
    //fColor = vec4(vCoord, 0, 1);
    fColor = texture(tex, vCoord);
    //if (vCoord.x > 1 || vCoord.x < 0) { fColor = vec4(1.f, 0.f, 0.f, 1.f); return; }
    //if (vCoord.y > 1 || vCoord.y < 0) { fColor = vec4(0.f, 1.f, 0.f, 1.f); return; }
    //fColor = vec4(1.f);
  }

  ).";

#endif //RAYTRACER_GLVIEWER_H
