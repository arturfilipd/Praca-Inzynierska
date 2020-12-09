#pragma once
#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

struct ShaderSource {
	std::string VertexSource;
	std::string FragmentSource;
};


class __declspec(dllexport) Shader {
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	Shader(const std::string& filename);
	~Shader();
	void Bind() const;
	void Unbind() const;
	inline unsigned int getID() const { return m_RendererID; };


	//Uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniformMatrix4f(const std::string& name, glm::mat4 &mat);

	
	
private:
	
	ShaderSource ParseShader(std::string filepath);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);	
	unsigned int GetUniformLocation(const std::string& name);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	
};
