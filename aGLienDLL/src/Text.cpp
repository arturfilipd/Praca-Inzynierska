#include "Text.h"
#include <GL\glew.h>
#include <iostream>
#include <GLFW\glfw3.h>
#include "App.h"

TextRenderer::TextRenderer(App * a){      
    app = a;  
    if (FT_Init_FreeType(&freetype))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }
    std::cout << "FREETYPE LIBRARY LAODED\n";   
    if (FT_New_Face(freetype, "res/fonts/arial.ttf", 0, &face)){
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return;
    }
    std::cout << "FREETYPE FACES LOADED\n";
    FT_Set_Pixel_Sizes(face, 0, 48);   
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
    if (FT_Init_FreeType(&freetype)) return;
    for (unsigned char c = 0; c < 128; c++)
    {       
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph " << c << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(c, character));
        //FT_Done_Face(face);
        //FT_Done_FreeType(freetype);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);  

    }
    GLFWwindow* window = app->GetWindow();
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    //glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
    std::cout << "Window: " << width << "x" << height << "\n";
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
    textShader = new Shader("res/shaders/Text.shader");
    textShader->Bind();
    textShader->SetUniformMatrix4f("projection", projection);
    FT_Done_Face(face);
    FT_Done_FreeType(freetype);
}


void TextRenderer::RenderText(std::string text, float x, float y, float size, glm::vec3 color, float depth){
    float scale = size / 48.f;   
    // activate corresponding render state	    
    textShader->Bind();
    textShader->SetUniform1f("u_Depth", depth);
    glUniform3f(glGetUniformLocation(textShader->getID(), "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    // iterate through all characters
    std::string::const_iterator c;
    int ww,wh;
    glfwGetWindowSize(app->GetWindow(), &ww, &wh);
    for (c = text.begin(); c != text.end(); c++)
    {
       
        Character ch = Characters[*c];        
        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
                    { xpos,     wh - (ypos + h),   0.0f, 1.0f },
                    { xpos,     wh - (ypos),       0.0f, 0.0f },
                    { xpos + w, wh - (ypos),       1.0f, 0.0f },

                    { xpos,     wh - (ypos + h),   0.0f, 1.0f },
                    { xpos + w, wh - (ypos),       1.0f, 0.0f },
                    { xpos + w, wh - (ypos + h),   1.0f, 1.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)        
    }
    
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
}
