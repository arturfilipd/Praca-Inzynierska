#include "Renderer.h"
#include <iostream>
#include "Game.h"
#include "Map.h"
#include "Texture.h"
#include <vector>
#include "Scene.h"
#include <glm\gtc\type_ptr.hpp>
void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        printf("%s::%d GLERROR: [%d] %s", file, line, error, gluErrorString(error));      
        return false;
    }
    return true;
}

Renderer::Renderer(){
    glm::mat4 ViewMatrix(1.0f);  
    glm::mat4 ProjectionMatrix(1.0f);
    ProjectionMatrix = glm::perspective(glm::radians(70.f), 16.f / 9.f, 0.1f, 1000.f);

    glm::mat4 ModelMatrix(1.f);
    ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.f, 0.f, 0.f));

    TextureShader = new Shader("res/shaders/Textured.shader");
    ModelShader = new Shader("res/shaders/Model.shader");
    BasicShader = new Shader("res/shaders/Basic.shader");
    UIShader = new Shader("res/shaders/UI.shader");
    BasicShader->Bind();
    BasicShader->SetUniformMatrix4f("u_ModelMatrix", ModelMatrix);
    BasicShader->SetUniformMatrix4f("u_ViewMatrix", ViewMatrix);
    BasicShader->SetUniformMatrix4f("u_ProjectionMatrix", ProjectionMatrix);
    BasicShader->SetUniform4f("u_Color", 0.1f, 0.8f, 0.2f, 1.0f);
    TextureShader->Bind();
    TextureShader->SetUniformMatrix4f("u_ModelMatrix", ModelMatrix);
    TextureShader->SetUniformMatrix4f("u_ViewMatrix", ViewMatrix);
    TextureShader->SetUniformMatrix4f("u_ProjectionMatrix", ProjectionMatrix);
    ModelShader->Bind();
    ModelShader->SetUniformMatrix4f("projection", ProjectionMatrix);
    UIShader->Bind();    
    glm::mat4 projection = glm::ortho(0.0f, 960.f, 540.f, 0.0f, -1.0f, 1.0f);
    UIShader->SetUniformMatrix4f("u_ProjectionMatrix", projection);    
    unsigned int wi[] = { 0,1,4,  1,5,4,  1,2,5,  2,6,5,  2,3,6,  3,7,6,  3,8,7,  8,9,7 };
    unsigned int ri[] = { 4,5,7,  5,6,7 };
    unsigned int fi[] = { 0,1,2, 2,3,0 };
    unsigned int fri[] = { 0,3,4,  3,7,4, 8,11,12, 11,15,12, 5,6,9, 6,10,9, 0,13,12, 0,1,13, 2,3,15, 2,15,14 };
    wallIB = new IndexBuffer(wi, 24);
    roofIB = new IndexBuffer(ri, 6);
    floorIB = new IndexBuffer(fi, 6);
    frameIB = new IndexBuffer(fri, 30);

    texturedLayout.Push<float>(3);
    texturedLayout.Push<float>(2);
    UILayout.Push<float>(2);
    UILayout.Push<float>(2);

}

Renderer::~Renderer()
{
    delete BasicShader;
    delete TextureShader;
    delete ModelShader;
}

void Renderer::Clear() const
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

}

void Renderer::Update(MenuScene * scene){
    GLFWwindow* window = scene->GetApp()->GetWindow();
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
    UIShader->Bind();
    UIShader->SetUniformMatrix4f("u_ProjectionMatrix", projection);
    UIShader->SetUniform1f("u_Depth", 0);
    VertexArray varray;
    varray.Bind();  
    for (UIElement* b : *(scene->GetChildren())) {
        Texture* tex = b->GetTexture();
        if (tex == NULL) continue;
        tex->Bind(0);      
        UIShader->SetUniform1i("u_Texture", 0);
        float data[] = {
            b->GetX(), b->GetY(), 0.f, 0.f, //0
            b->GetX() + b->GetWidth(), b->GetY(), 1.f, 0.f, //1
            b->GetX() + b->GetWidth(), b->GetY()+b->GetHeight(), 1.f, 1.f, //2
            b->GetX(), b->GetY()+b->GetHeight(), 0.f, 1.f, //3
        }; 
        VertexBuffer b(data,4 * 4 * sizeof(float));        
        varray.AddBuffer(b, UILayout);
        Draw(varray, *floorIB, *UIShader);
   }
}


void Renderer::Update(Game * game, Camera& cam)
{
    glm::mat4 vm = cam.GetViewMatrix();
    
    //Setup
    ModelShader->Bind();
    ModelShader->SetUniformMatrix4f("view", vm);
    BasicShader->Bind();    
    BasicShader->SetUniformMatrix4f("u_ViewMatrix", vm);
    TextureShader->Bind();
    TextureShader->SetUniformMatrix4f("u_ViewMatrix", vm);
    

    //Terrain
    VertexArray varray;
    varray.Bind();
    Map* map = game->GetMap();
    
    for (int x = 0; x < map->GetSize(); x++) {
        for (int y = 0; y < map->GetSize(); y++) {  
            TextureShader->Bind();
            Tile* t = map->GetTile(x, y);
            Texture* tex = map->GetTexture((TILETYPE)t->GetType());           
            tex->Bind(0);
            TextureShader->SetUniform1i("u_Texture", 0);            
            TileModel* data = t->GetVertices();
            VertexBuffer b(data->vertices, data->verticesCount * 5 * sizeof(float));
            IndexBuffer* i = floorIB;
            if (t->GetType() == WALL) i = wallIB;        
            varray.AddBuffer(b, texturedLayout);
            Draw(varray, *i, *TextureShader);            
            if (t->GetType() == WALL) {
                BasicShader->Bind();
                BasicShader->SetUniform4f("u_Color", 0.48f, 0.48f, 0.48f, 1.f);
                VertexBuffer roofB(data->vertices, 10 * 5 * sizeof(float));
                varray.AddBuffer(roofB, texturedLayout);                
                Draw(varray, *roofIB, *BasicShader);
            }               
            delete data;           
        }
    }
    
    //Entities
    ModelShader->Bind();
    std::list<Entity*>* entities = game->GetEntities();    
    for (Entity* e : *entities) {
        if (e->GetModel() == NULL) continue;
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(e->GetX(), 0.f, e->GetY()));
        glm::vec2 r = e->GetRotation();
        if (r.x != 0 || r.y != 0)
            model = glm::rotate(model, atan2f(-r.y, r.x) + glm::radians(90.f), glm::vec3(0.f, 1.0f, 0.f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        ModelShader->SetUniformMatrix4f("model", model);
        e->GetModel()->Draw(*ModelShader);
    }
}

void Renderer::Update(UIElement* e){
    Texture* tex = e->GetTexture();
    if (tex == NULL) return;
    GLFWwindow* window = e->GetScene()->GetApp()->GetWindow();
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
    UIShader->Bind();
    VertexArray varray;
    varray.Bind();
    UIShader->SetUniformMatrix4f("u_ProjectionMatrix", projection);    
    tex->Bind(0);
    UIShader->SetUniform1i("u_Texture", 0);
    UIShader->SetUniform1f("u_Depth", e->GetDepth());
   
    if (!e->isFrame()) {
        float x = e->GetX();
        float mx = e->GetX() + e->GetWidth();
        float y = e->GetY();
        float my = e->GetY() + e->GetHeight();
        float data[] = {
        x,  y,  0.f, 1.f, //0
        mx, y,  1.f, 1.f, //1
        mx, my, 1.f, 0.f, //2
        x,  my, 0.f, 0.f, //3
        };
        VertexBuffer b(data, 4 * 4 * sizeof(float));
        varray.AddBuffer(b, UILayout);
        Draw(varray, *floorIB, *UIShader);
    }
    else {
        float x = e->GetX();
        float y = e->GetY();
        float framesize = e->GetFrameSize();
        float data[] = {
        //Top
        x,                              y,                                  0.0f, 1.0f, //0
        x + framesize,                  y,                                  0.1f, 1.0f, //1
        x - framesize + e->GetWidth(),  y,                                  0.9f, 1.0f, //2
        x + e->GetWidth(),              y,                                  1.0f, 1.0f, //3
        //Top-Middle
        x,                              y + framesize,                      0.0f, 0.9f, //4
        x + framesize,                  y + framesize,                      0.1f, 0.9f, //5
        x - framesize + e->GetWidth(),  y + framesize,                      0.9f, 0.9f, //6
        x + e->GetWidth(),              y + framesize,                      1.0f, 0.9f, //7
        //Bottom-Middle
        x,                              y - framesize + e->GetHeight(),     0.0f, 0.1f, //8
        x + framesize,                  y - framesize + e->GetHeight(),     0.1f, 0.1f, //9
        x - framesize +e->GetWidth(),   y - framesize +e->GetHeight(),      0.9f, 0.1f, //10
        x + e->GetWidth(),              y - framesize + e->GetHeight(),     1.0f, 0.1f, //11
        //Bottom
        x,                              y + e->GetHeight(),                 0.0f, 0.0f, //12
        x + framesize,                  y + e->GetHeight(),                 0.1f, 0.0f, //13
        x - framesize + e->GetWidth(),  y + e->GetHeight(),                 0.9f, 0.0f, //14
        x + e->GetWidth(),              y + e->GetHeight(),                 1.0f, 0.0f, //15
        };
        VertexBuffer b(data, 16 * 4 * sizeof(float));
        varray.AddBuffer(b, UILayout);
        Draw(varray, *frameIB, *UIShader);
    }


}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const{
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}