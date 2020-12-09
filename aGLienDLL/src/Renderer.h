#pragma once

#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Entity.h"
#include "list"
#include "Shader.h"
#include "Entity.h"
#include "Camera.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Scene.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__ ));

class Game;

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class __declspec(dllexport) Renderer {
private:
	Shader * TextureShader;
	Shader * ModelShader;
	Shader * BasicShader;
	Shader * UIShader;
	VertexBufferLayout texturedLayout;
	VertexBufferLayout UILayout;
	IndexBuffer* wallIB;
	IndexBuffer* roofIB;
	IndexBuffer* floorIB;
	IndexBuffer* frameIB;

public:
	Renderer();
	~Renderer();

	void Clear() const;
	void Update(Game *g, Camera&);
	void Update(UIElement* e);
	void Update(MenuScene* scene);
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};