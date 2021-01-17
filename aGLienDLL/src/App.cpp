

#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include "App.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm\gtc\type_ptr.hpp>
#include "Camera.h"

#include <ctime>
#include "Texture.h"
#include "Tile.h"
#include "Map.h"
#include "Model.h"
#include "Scene.h"



class Player;

App::App(){}

App::App(const char * n){
    appName = n;
}

int App::Init() {
    srand(time(NULL));   
 
    if (!glfwInit()) return -1;

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    float window_width = mode->width;
    float window_height = mode->height;

    //glfwGetPrimaryMonitor()
    window =  glfwCreateWindow(window_width, window_height, appName.c_str(), glfwGetPrimaryMonitor(), NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (glewInit() != GLEW_OK)
        std::cout << "GLEW INIT ERROR";
    renderer = new Renderer();
    textRenderer = new TextRenderer(this);
}

int App::Run (MenuScene * scene){     
    currentScene = scene;
    prevTime = glfwGetTime();
    while (!exit && glfwWindowShouldClose){  
        timeDiff = glfwGetTime() - prevTime;
        if (timeDiff > 1.0 / 180.0) {
            prevTime = glfwGetTime();
            currentScene->Update();
        }   
        if (sceneChange) {        
            currentScene = nextScene;
            sceneChange = false;           
        }
        renderer->Clear();
        currentScene->Draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
       
    } 
    glfwTerminate();
    return 0;
}

void App::ChangeScene(Scene* newScene){
    if (newScene) {
        sceneChange = true;
        nextScene = newScene;
    }
}

void App::Close(){
    exit = true;
}

double App::GetCurrentTime(){
    return glfwGetTime();
}

int App::GetKeyStatus(int keyCode){
    return glfwGetKey(window, keyCode);
}
