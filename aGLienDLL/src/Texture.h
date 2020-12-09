#pragma once
#include "Renderer.h"

class __declspec(dllexport) Texture {
private:
	unsigned int RendererID;
	std::string filePath;
	unsigned char* localBuffer;
	int width, height, BPP;
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind();

	inline int getWidth() const { return width; }
	inline int getHeight() const { return height; }
};