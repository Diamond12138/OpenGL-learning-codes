#pragma once
#ifndef utilities_hpp
#define utilities_hpp

#include <string>
#include <sstream>
#include <fstream>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>

enum LogLevel
{
    INFO,
    ERROR
};

inline std::string toString();
template <typename T> inline std::string toString(T value);
inline std::string toStrings();
template <typename T, typename... Ts> inline std::string toStrings(T arg, Ts... args);
std::string loadStringFromStream(std::istream &is);
std::string loadStringFromFile(std::string filename);
enum LogLevel;
std::string getLevel(LogLevel level);
template <typename... Ts> void log(LogLevel level, Ts... args);
template <typename... Ts> void logi(Ts... args);
template <typename... Ts> void loge(Ts... args);
template <typename T> std::string toStringHex(T value);
std::string getSource(GLenum source);
std::string getType(GLenum type);
std::string getID(GLenum id);
std::string getSeverity(GLenum severity);
void GLAPIENTRY onMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
void initGL(sf::Window& window);



inline std::string toString()
{
    return std::string();
}

template <typename T>
inline std::string toString(T value)
{
    std::stringstream sstr;
    sstr << value;
    return sstr.str();
}

inline std::string toStrings()
{
    return toString();
}

template <typename T, typename... Ts>
inline std::string toStrings(T arg, Ts... args)
{
    return toString(arg) + toStrings(args...);
}

std::string loadStringFromStream(std::istream &is)
{
    std::stringstream sstr;
    std::string temp;
    try
    {
        while (getline(is, temp))
        {
            sstr << temp << std::endl;
        }
    }
    catch (const std::istream::failure error)
    {
        loge(error.what());
    }
    return sstr.str();
}

std::string loadStringFromFile(std::string filename)
{
    std::ifstream fin;
    fin.open(filename);
    if (fin.fail())
    {
        loge("Unable to open file:\"", filename, "\"");
        return std::string();
    }
    std::string result = loadStringFromStream(fin);
    fin.close();
    return result;
}

std::string getLevel(LogLevel level)
{
    std::string result;

    switch (level)
    {
    case LogLevel::INFO:
        result = "INFO";
        break;

    case LogLevel::ERROR:
        result = "ERROR";
        break;

    default:
        result = toString(level);
        break;
    }

    return result;
}

template <typename... Ts>
void log(LogLevel level, Ts... args)
{
    std::cout << "[" << getLevel(level) << "]" << toStrings(args...) << std::endl;
}

template <typename... Ts>
void logi(Ts... args)
{
    log(LogLevel::INFO,args...);
}

template <typename... Ts>
void loge(Ts... args)
{
    log(LogLevel::ERROR,args...);
}

template <typename T>
std::string toStringHex(T value)
{
    std::stringstream sstr;
    sstr << "0x" << std::hex << value << std::dec;
    return sstr.str();
}

std::string getSource(GLenum source)
{
    std::string result;
    if (source == GL_DEBUG_SOURCE_API)
        result = "GL_DEBUG_SOURCE_API";
    else if (source == GL_DEBUG_SOURCE_APPLICATION)
        result = "GL_DEBUG_SOURCE_APPLICATION";
    else if (source == GL_DEBUG_SOURCE_OTHER)
        result = "GL_DEBUG_SOURCE_OTHER";
    else if (source == GL_DEBUG_SOURCE_SHADER_COMPILER)
        result = "GL_DEBUG_SOURCE_SHADER_COMPILER";
    else if (source == GL_DEBUG_SOURCE_THIRD_PARTY)
        result = "GL_DEBUG_SOURCE_THIRD_PARTY";
    else if (source == GL_DEBUG_SOURCE_WINDOW_SYSTEM)
        result = "GL_DEBUG_SOURCE_WINDOW_SYSTEM";
    else
        result = toStringHex(source);
    return result;
}

std::string getType(GLenum type)
{
    std::string result;
    if (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR)
        result = "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
    else if (type == GL_DEBUG_TYPE_ERROR)
        result = "GL_DEBUG_TYPE_ERROR";
    else if (type == GL_DEBUG_TYPE_MARKER)
        result = "GL_DEBUG_TYPE_MARKER";
    else if (type == GL_DEBUG_TYPE_OTHER)
        result = "GL_DEBUG_TYPE_OTHER";
    else if (type == GL_DEBUG_TYPE_PERFORMANCE)
        result = "GL_DEBUG_TYPE_PERFORMANCE";
    else if (type == GL_DEBUG_TYPE_POP_GROUP)
        result = "GL_DEBUG_TYPE_POP_GROUP";
    else if (type == GL_DEBUG_TYPE_PORTABILITY)
        result = "GL_DEBUG_TYPE_PORTABILITY";
    else if (type == GL_DEBUG_TYPE_PUSH_GROUP)
        result = "GL_DEBUG_TYPE_PUSH_GROUP";
    else if (type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR)
        result = "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
    else
        result = toStringHex(type);
    return result;
}

std::string getID(GLenum id)
{
    std::string result;
    switch (id)
    {
    case GL_NO_ERROR:
        result = "GL_NO_ERROR";
        break;
    case GL_INVALID_ENUM:
        result = "GL_INVALID_ENUM";
        break;
    case GL_INVALID_VALUE:
        result = "GL_INVALID_VALUE";
        break;
    case GL_INVALID_OPERATION:
        result = "GL_INVALID_OPERATION";
        break;
    case GL_STACK_OVERFLOW:
        result = "GL_STACK_OVERFLOW";
        break;
    case GL_STACK_UNDERFLOW:
        result = "GL_STACK_UNDERFLOW";
        break;
    case GL_OUT_OF_MEMORY:
        result = "GL_OUT_OF_MEMORY";
        break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        result = "GL_INVALID_FRAMEBUFFER_OPERATION";
        break;
    default:
        result = toStringHex(id);
        break;
    }
    return result;
}

std::string getSeverity(GLenum severity)
{
    std::string result;
    if (severity == GL_DEBUG_SEVERITY_HIGH)
        result = "GL_DEBUG_SEVERITY_HIGH";
    else if (severity == GL_DEBUG_SEVERITY_MEDIUM)
        result = "GL_DEBUG_SEVERITY_MEDIUM";
    else if (severity == GL_DEBUG_SEVERITY_LOW)
        result = "GL_DEBUG_SEVERITY_LOW";
    else
        result = toStringHex(severity);
    return result;
}

void GLAPIENTRY onMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    std::stringstream sstr;
    sstr << std::endl
         << std::hex
         << "source:" << getSource(source) << std::endl
         << "type:" << getType(type) << std::endl
         << "id:" << getID(id) << std::endl
         << "severity:" << getSeverity(severity) << std::endl
         << std::dec
         << "length:" << length << std::endl
         << "message:" << message << std::endl
         << "user param:" << userParam << std::endl;
    loge(sstr.str());
}

void initGL(sf::Window& window)
{
    if(glewInit() == GLEW_OK)
    {
        logi("OpenGL loaded successfully.");
    }
    else
    {
        loge("OpenGL load failed.");
        window.close();
    }
}

#endif