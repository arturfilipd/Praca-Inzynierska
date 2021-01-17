/*
Kod zaczerpnięty i edytowany na licencji CC BY-NC 4.0
Źródło: https://learnopengl.com/In-Practice/Text-Rendering
Autor: Joey de Vries (twitter.com/JoeyDeVriez)
Licencja: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#pragma once
#include "Shader.h"
#include <map>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H  

class App;

struct Character {    
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

class TextRenderer {
    FT_Library freetype;
    FT_Face face;   
    App * app;
    std::map<char, Character> Characters;
    unsigned int VAO, VBO;
    Shader* textShader;
    
    
public:
    TextRenderer(App * a);    
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color, float depth);
    
};