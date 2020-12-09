#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include <glm\gtc\type_ptr.hpp>


Shader::Shader(const std::string& filename)
	:m_FilePath(filename), m_RendererID(0)
{
    ShaderSource source = ParseShader(filename);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniformMatrix4f(const std::string& name, glm::mat4 &mat)
{ 
    float dArray[16] = { 0.0 };    
    const float* pSource = (const float*)glm::value_ptr(mat);
    for (int i = 0; i < 16; ++i)
        dArray[i] = pSource[i];      
        
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, dArray);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
   
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
   
}

void Shader::SetUniform1i(const std::string& name, int value){
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value) {
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) 
        return m_UniformLocationCache[name];    
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform " << name << " not found!" << std::endl;
    else
        m_UniformLocationCache[name] = location;
	return location;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* msg = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, msg);
        std::cout << "Failed to compile shader: " << std::endl;
        std::cout << msg << std::endl;
    }
    return id;
}


unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

ShaderSource Shader::ParseShader(std::string filepath) {
    std::ifstream stream(m_FilePath);
    std::string line;
    enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };
    std::stringstream ss[2];
    ShaderType mode = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                mode = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                mode = ShaderType::FRAGMENT;
        }
        else
            ss[(int)mode] << line << '\n';
    }
    return { ss[0].str(), ss[1].str() };
}