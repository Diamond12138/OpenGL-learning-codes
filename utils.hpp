#pragma once

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <fstream>
#include <sstream>

namespace tr
{
    static std::ofstream flog;

    /// @brief OpenGL的调试信息回调函数
    void debug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
    {
        flog << message << std::endl;
    }

    /// @brief 初始化OpenGL函数
    bool initGL()
    {
        bool success = glewInit() == GLEW_OK;
        if (success)
        {
            flog << "GL initialized successfully" << std::endl;
            glDebugMessageCallback(debug, nullptr);
        }
        else
        {
            flog << "Failed to initialize GL" << std::endl;
        }
        return success;
    }

    /// @brief 字形
    struct Glyph
    {
        /// @brief 纹理
        GLuint texture;
        /// @brief 大小（单位：像素）
        glm::ivec2 size;
        /// @brief 到基准线的距离（单位：像素）
        glm::ivec2 bearing;
        /// @brief 到下一字形的距离（单位：1/64像素）
        glm::ivec2 advance;

        /// @brief 构造函数
        Glyph(FT_Face face)
        {
            // 初始化纹理
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // 设置参数
            size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
            bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
            advance = glm::ivec2(face->glyph->advance.x, face->glyph->advance.y) / 64;
        }
    };

    /// @brief 加载所有ASCII字形
    bool loadGlyphs(const std::string &filename, size_t size, std::map<char, Glyph> &result)
    {
        // 初始化FreeType
        FT_Library lib;
        if (FT_Init_FreeType(&lib))
        {
            flog << "Unable to initialize FreeType" << std::endl;
            return false;
        }

        // 加载Face
        FT_Face face;
        if (FT_New_Face(lib, filename.data(), 0, &face))
        {
            flog << "Unable to load Face" << std::endl;
            FT_Done_FreeType(lib);
            return false;
        }

        // 设置字体大小
        FT_Set_Pixel_Sizes(face, 0, size);

        // 加载字形
        result.clear();
        for (size_t i = 0; i < 128; i++)
        {
            if (FT_Load_Char(face, i, FT_LOAD_RENDER))
            {
                flog << "Unable to load glyph:\'" << i << "\'" << std::endl;
                continue;
            }
            else
            {
                flog << "Glyph:\'" << i << "\' loaded successfully" << std::endl;
                result.insert(std::pair<char, Glyph>(i, Glyph(face)));
            }
        }

        // 释放资源
        FT_Done_Face(face);
        FT_Done_FreeType(lib);

        return true;
    }

    /// @brief 从文件加载字符串
    std::string loadStringFromFile(const std::string &filename)
    {
        // 打开文件
        std::ifstream fin;
        fin.open(filename);
        if (fin.fail())
        {
            flog << "Could not open file:\"" << filename << "\"" << std::endl;
            fin.close();
            return "";
        }

        // 读入文本
        std::stringstream sstr;
        std::string temp;
        while (std::getline(fin, temp))
        {
            sstr << temp << std::endl;
        }

        // 关闭文件
        fin.close();

        return sstr.str();
    }

    /// @brief 从文件加载着色器
    GLuint loadShaderFromFile(const std::string &filename, GLenum type)
    {
        // 创建着色器
        std::string source = loadStringFromFile(filename);
        const char *sourceptr = source.data();
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &sourceptr, nullptr);
        glCompileShader(shader);

        // 检查着色器
        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success = GL_FALSE)
        {
            flog << "Failed to compile shader" << std::endl;
            glDeleteShader(shader);
            shader = 0;
        }
        else
        {
            flog << "Shader compiled successfully" << std::endl;
        }

        return shader;
    }

    /// @brief 从文件加载着色程序
    GLuint loadProgramFromFile(const std::string &vfilename, const std::string &ffilename)
    {
        // 加载着色器
        GLuint vshader = loadShaderFromFile(vfilename, GL_VERTEX_SHADER);
        GLuint fshader = loadShaderFromFile(ffilename, GL_FRAGMENT_SHADER);
        if (vshader == 0 || fshader == 0)
        {
            return 0;
        }

        // 创建着色程序
        GLuint program = glCreateProgram();
        glAttachShader(program, vshader);
        glAttachShader(program, fshader);
        glLinkProgram(program);

        // 检查着色程序
        int success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (success = GL_FALSE)
        {
            flog << "Linking program failed" << std::endl;
            glDeleteProgram(program);
            program = 0;
        }
        else
        {
            flog << "Program link succeeded" << std::endl;
        }

        // 删除着色器
        glDeleteShader(vshader);
        glDeleteShader(fshader);

        return program;
    }

    /// @brief 创建VBO
    GLuint createVBO(const std::vector<GLfloat> &vertices, GLuint index)
    {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(index);
        return vbo;
    }
} // namespace tr
