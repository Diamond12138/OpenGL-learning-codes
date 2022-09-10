#pragma once
#ifndef utilities_hpp
#define utilities_hpp

#include <string>
#include <sstream>
#include <fstream>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum LogLevel
{
    INFO,
    ERROR
};

namespace VertexAttribLocation
{
    enum VertexAttribLocation
    {
        VERTEX = 0,
        COLOR = 1,
        NORMAL = 2,
        TEX_COORD = 3,
        COUNT
    };
}

namespace UniformLocation
{
    enum UniformLocation
    {
        PROJECTION = 0,
        VIEW = 1,
        MODEL = 2,
        CAMERA_POSITION = 3,
        COLOR = 4,
        COUNT
    };
}

struct Model
{
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
};

struct Image
{
    int width = 0;
    int height = 0;
    int channels_in_file = 0;
    unsigned char *pixels = nullptr;
};

struct MatrixHolder
{
    glm::mat4 matrix;
};

struct Transform : public MatrixHolder
{
    glm::vec3 translate;
    glm::vec3 rotate;
    glm::vec3 scale;

    Transform(glm::vec3 t = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 r = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 s = glm::vec3(1.0f, 1.0f, 1.0f)) : translate(t), rotate(r), scale(s) {}
};

struct View : public MatrixHolder
{
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    View(glm::vec3 p = glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3 f = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 u = glm::vec3(0.0f, 1.0f, 0.0f)) : position(p), front(f), up(u) {}
};

struct Projection : public MatrixHolder
{
};

struct PerspectiveProjection : public Projection
{
    float fovy;
    float aspect;
    float zNear;
    float zFar;

    PerspectiveProjection(float f = 60.0f, float a = 1.0f, float zn = 0.1f, float zf = 100.0f) : fovy(f), aspect(a), zNear(zn), zFar(zf) {}
};

struct OrthoProjection : public Projection
{
    float left;
    float right;
    float bottom;
    float top;
    float zNear;
    float zFar;

    OrthoProjection(float l = -1.0f, float r = 1.0f, float b = -1.0f, float t = 1.0f, float zn = 0.1f, float zf = 100.0f) : left(l), right(r), bottom(b), top(t), zNear(zn), zFar(zf) {}
};

struct Camera
{
    virtual glm::mat4 getProjection() = 0;
    virtual glm::mat4 getView() = 0;
};

struct PerspectiveCamera : public Camera, public PerspectiveProjection, public View
{
    virtual glm::mat4 getProjection()
    {
        return PerspectiveProjection::matrix;
    }

    virtual glm::mat4 getView()
    {
        return View::matrix;
    }
};

struct Shadow
{
    GLuint FBO = 0;
    GLuint RBO = 0;

    Projection projection;
    View view;
};

struct DirectionalLightShadow : public Shadow
{
    GLuint texture = 0;
};

struct RenderableObject : public Transform
{
    GLuint VAO = 0;
    GLsizei vertex_count = 0;

    RenderableObject(GLuint vao, GLsizei vc, glm::vec3 t = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 r = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 s = glm::vec3(1.0f, 1.0f, 1.0f)) : Transform(t, r, s), VAO(vao), vertex_count(vc) {}

    virtual glm::mat4 getModel()
    {
        return matrix;
    }
};

inline std::string toString();
template <typename T>
inline std::string toString(T value);
inline std::string toStrings();
template <typename T, typename... Ts>
inline std::string toStrings(T arg, Ts... args);
std::string loadStringFromStream(std::istream &is);
std::string loadStringFromFile(std::string filename);
std::string getLevel(LogLevel level);
template <typename... Ts>
void log(LogLevel level, Ts... args);
template <typename... Ts>
void logi(Ts... args);
template <typename... Ts>
void loge(Ts... args);
template <typename T>
std::string toStringHex(T value);
std::string getSource(GLenum source);
std::string getType(GLenum type);
std::string getID(GLenum id);
std::string getSeverity(GLenum severity);
void GLAPIENTRY onMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
void initGL(sf::Window &window);
GLuint compileShader(GLenum type, std::string source);
GLuint linkProgram(GLuint vshader, GLuint fshader);
GLuint loadProgram(std::string vfilename, std::string ffilename);
GLuint bindVBO(GLuint location, GLint size, std::vector<float> vertices, GLenum usage = GL_STATIC_DRAW);
GLuint createVAO(Model model, GLenum usage = GL_STATIC_DRAW);
Model generatePlane(glm::vec2 size = glm::vec2(1.0f, 1.0f));
bool needQuit(sf::Event event);
Image loadImage(std::string filename, int desired_channels = 0);
void releaseImage(Image &image);
GLuint bindTexture2D(Image image);
GLuint createTexture2D(std::string filename);
void updateTransform(Transform &transform);
void updateView(View &view);
void updatePerspectiveProjection(PerspectiveProjection &projection);
GLint getUniformLocation(GLuint program, std::string name);
Model generatePlane_2DLighting(glm::vec2 size = glm::vec2(1.0f, 1.0f));
Model generateCube(glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f));
GLuint createVolumeTexture(unsigned int subdivision = 8, glm::vec4 color = glm::vec4(0.1f, 0.1f, 0.1f, 0.1f));
GLuint createVAO_3DTexture(Model model, GLenum usage = GL_STATIC_DRAW);
GLuint createTexture2D_empty();
GLuint createFBO();
GLuint createRBO();
DirectionalLightShadow createDLShadow();
void updateOrthoProjection(OrthoProjection &projection);
void deleteShadow(Shadow &shadow);
void deleteDLShadow(DirectionalLightShadow &shadow);
Model generateAxis(float size = 100.0f);

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
    log(LogLevel::INFO, args...);
}

template <typename... Ts>
void loge(Ts... args)
{
    log(LogLevel::ERROR, args...);
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
    if (severity == GL_DEBUG_SEVERITY_HIGH)
    {
        exit(EXIT_FAILURE);
    }
}

void initGL(sf::Window &window)
{
    if (glewInit() == GLEW_OK)
    {
        logi("OpenGL loaded successfully.");
        glDebugMessageCallback(onMessage, nullptr);
    }
    else
    {
        loge("OpenGL load failed.");
        window.close();
    }
}

GLuint compileShader(GLenum type, std::string source)
{
    std::string type_str;
    switch (type)
    {
    case GL_VERTEX_SHADER:
        type_str = "vertex shader";
        break;

    case GL_FRAGMENT_SHADER:
        type_str = "fragment shader";
        break;

    default:
        type_str = "Unknown";
        loge("Unknown shader type,value:", type);
        break;
    }

    const char *ptr = source.data();
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &ptr, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == 1)
    {
        logi("Successfully compiled shader,type:", type_str, ",value:", shader);
    }
    else
    {
        loge("Shader compilation failed.");
        glDeleteShader(shader);
        shader = 0;
    }
    return shader;
}

GLuint linkProgram(GLuint vshader, GLuint fshader)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vshader);
    glAttachShader(program, fshader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == 1)
    {
        logi("Successfully linked program,value:", program);
    }
    else
    {
        loge("Program linking failed.");
        glDeleteProgram(program);
        program = 0;
    }
    return program;
}

GLuint loadProgram(std::string vfilename, std::string ffilename)
{
    GLuint vshader = compileShader(GL_VERTEX_SHADER, loadStringFromFile(vfilename));
    GLuint fshader = compileShader(GL_FRAGMENT_SHADER, loadStringFromFile(ffilename));
    GLuint program = linkProgram(vshader, fshader);
    glDeleteShader(vshader);
    glDeleteShader(fshader);
    return program;
}

GLuint bindVBO(GLuint location, GLint size, std::vector<float> vertices, GLenum usage)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), usage);
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, size * sizeof(GLfloat), nullptr);

    return vbo;
}

GLuint createVAO(Model model, GLenum usage)
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo1 = bindVBO(VertexAttribLocation::VERTEX, 3, model.vertices, usage);
    GLuint vbo2 = bindVBO(VertexAttribLocation::NORMAL, 3, model.normals, usage);
    GLuint vbo3 = bindVBO(VertexAttribLocation::TEX_COORD, 2, model.texCoords, usage);

    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo1);
    glDeleteBuffers(1, &vbo2);
    glDeleteBuffers(1, &vbo3);

    return vao;
}

Model generatePlane(glm::vec2 size)
{
    const glm::vec2 half = size / 2.0f;
    std::vector<float> vertices =
        {
            -half.x, half.y, 0.0f,
            -half.x, -half.y, 0.0f,
            half.x, -half.y, 0.0f,

            -half.x, half.y, 0.0f,
            half.x, -half.y, 0.0f,
            half.x, half.y, 0.0f};
    std::vector<float> normals =
        {
            0.0f,
            0.0f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,

            0.0f,
            0.0f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,
            0.0f,
            0.0f,
            1.0f,
        };
    std::vector<float> texCoords =
        {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,

            0.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 0.0f};
    Model result = {vertices, normals, texCoords};
    return result;
}

Model generatePlane_2DLighting(glm::vec2 size)
{
    const glm::vec2 half = size / 2.0f;
    std::vector<float> vertices =
        {
            -half.x, half.y, 0.0f,
            0.0f, 0.0f, 0.0f,
            half.x, half.y, 0.0f,

            -half.x, half.y, 0.0f,
            -half.x, -half.y, 0.0f,
            0.0f, 0.0f, 0.0f,

            0.0f, 0.0f, 0.0f,
            -half.x, -half.y, 0.0f,
            half.x, -half.y, 0.0f,

            0.0f, 0.0f, 0.0f,
            half.x, -half.y, 0.0f,
            half.x, half.y, 0.0f};
    std::vector<float> normals =
        {
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,

            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,

            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f};
    std::vector<float> texCoords =
        {
            0.0f, 0.0f,
            0.5f, 0.5f,
            1.0f, 0.0f,

            0.0f, 0.0f,
            0.0f, 1.0f,
            0.5f, 0.5f,

            0.5f, 0.5f,
            0.0f, 1.0f,
            1.0f, 1.0f,

            0.5f, 0.5f,
            1.0f, 1.0f,
            1.0f, 0.0f};
    Model result = {vertices, normals, texCoords};
    return result;
}

bool needQuit(sf::Event event)
{
    return (event.type == sf::Event::EventType::Closed) ||
           ((event.type == sf::Event::EventType::KeyPressed) &&
            (event.key.code == sf::Keyboard::Key::Escape));
}

Image loadImage(std::string filename, int desired_channels)
{
    Image result;
    result.pixels = stbi_load(filename.data(), &result.width, &result.height, &result.channels_in_file, desired_channels);
    return result;
}

void releaseImage(Image &image)
{
    image.width = 0;
    image.height = 0;
    image.channels_in_file = 0;
    if (image.pixels != nullptr)
    {
        stbi_image_free(image.pixels);
    }
    image.pixels = nullptr;
}

GLuint bindTexture2D(Image image)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (image.pixels != nullptr)
    {
        GLenum format;
        switch (image.channels_in_file)
        {
        case 1:
            format = GL_RED;
            break;
        case 2:
            format = GL_RG;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            loge("channels_in_file is invalid:", image.channels_in_file);
            break;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, image.width, image.height, 0, format, GL_UNSIGNED_BYTE, image.pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        loge("Pointer to pixels is null.");
    }

    return texture;
}

GLuint createTexture2D(std::string filename)
{
    Image image = loadImage(filename);
    GLuint result = bindTexture2D(image);
    releaseImage(image);
    return result;
}

void updateTransform(Transform &transform)
{
    glm::mat4 matrix = glm::mat4(1.0f);
    matrix = glm::translate(matrix, transform.translate);
    matrix = glm::rotate(matrix, transform.rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::rotate(matrix, transform.rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::rotate(matrix, transform.rotate.z, glm::vec3(0.0f, 0.0f, 1.0f));
    matrix = glm::scale(matrix, transform.scale);
    transform.matrix = matrix;
}

void updateView(View &view)
{
    view.front = glm::normalize(view.front);
    view.matrix = glm::lookAt(view.position, view.position + view.front, view.up);
}

void updatePerspectiveProjection(PerspectiveProjection &projection)
{
    projection.matrix = glm::perspective(projection.fovy, projection.aspect, projection.zNear, projection.zFar);
}

GLint getUniformLocation(GLuint program, std::string name)
{
    GLint location = glGetUniformLocation(program, name.data());
    if (location == -1)
    {
        loge("Unable to find uniform:\"", name, "\"");
    }
    else
    {
        logi("Successfully found uniform:\"", name, "\",value:", location);
    }
    return location;
}

Model generateCube(glm::vec3 size)
{
    const glm::vec3 half = size / 2.0f;

    std::vector<float> vertices =
        {

        };

    std::vector<float> normals =
        {

        };

    std::vector<float> texCoords =
        {

        };

    Model result = {vertices, normals, texCoords};
    return result;
}

GLuint createVolumeTexture(unsigned int subdivision, glm::vec4 color)
{
    const glm::vec4 color256 = color * 255.0f;
    const glm::uvec4 ucolor = glm::uvec4(GLubyte(color256.x), GLubyte(color256.y), GLubyte(color256.z), GLubyte(color256.w));

    std::vector<GLubyte> pixels;
    for (unsigned int i = 0; i < subdivision; i++)
    {
        for (unsigned int j = 0; j < subdivision; j++)
        {
            for (unsigned int k = 0; k < subdivision; k++)
            {
                pixels.push_back(ucolor.x);
                pixels.push_back(ucolor.y);
                pixels.push_back(ucolor.z);
                pixels.push_back(ucolor.w);
            }
        }
    }

    GLuint texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_3D, texture);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, subdivision, subdivision, subdivision, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    // glGenerateMipmap(GL_TEXTURE_3D);

    return texture;
}

GLuint createVAO_3DTexture(Model model, GLenum usage)
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo1 = bindVBO(VertexAttribLocation::VERTEX, 3, model.vertices, usage);
    GLuint vbo2 = bindVBO(VertexAttribLocation::NORMAL, 3, model.normals, usage);
    GLuint vbo3 = bindVBO(VertexAttribLocation::TEX_COORD, 3, model.texCoords, usage);

    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo1);
    glDeleteBuffers(1, &vbo2);
    glDeleteBuffers(1, &vbo3);

    return vao;
}

GLuint createTexture2D_empty(GLsizei width, GLsizei height, GLint format)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return texture;
}

GLuint createFBO()
{
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return fbo;
}

GLuint createRBO()
{
    return 0;
}

DirectionalLightShadow createDLShadow(glm::vec3 position, glm::vec3 direction, float zFar, unsigned int width = 1024, unsigned int height = 1024, glm::vec2 viewport = glm::vec2(2.0f, 2.0f))
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLuint fbo = createFBO();
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status == GL_FRAMEBUFFER_COMPLETE)
    {
        logi("The FBO of DirectionalLightShadow is complete.");
    }
    else
    {
        loge("The FBO of DirectionalLightShadow is complete,value:", status);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glm::vec2 half = viewport / 2.0f;

    OrthoProjection p;
    p.left = -half.x;
    p.right = half.x;
    p.bottom = -half.y;
    p.top = half.y;
    p.zNear = 0.1f;
    p.zFar = zFar;
    updateOrthoProjection(p);

    DirectionalLightShadow result;
    result.FBO = fbo;
    result.RBO = createRBO();
    result.projection.matrix = p.matrix;
    result.view = View(position, direction, glm::vec3(0.0f, 1.0f, 0.0f));
    result.texture = texture;
    updateView(result.view);

    return result;
}

void updateOrthoProjection(OrthoProjection &projection)
{
    projection.matrix = glm::ortho(projection.left, projection.right, projection.bottom, projection.top, projection.zNear, projection.zFar);
}

void deleteShadow(Shadow &shadow)
{
    if (shadow.FBO != 0)
    {
        glDeleteFramebuffers(1, &shadow.FBO);
        shadow.FBO = 0;
    }

    if (shadow.RBO != 0)
    {
        glDeleteRenderbuffers(1, &shadow.RBO);
        shadow.RBO = 0;
    }
}

void deleteDLShadow(DirectionalLightShadow &shadow)
{
    deleteShadow(shadow);

    if (shadow.texture != 0)
    {
        glDeleteTextures(1, &shadow.texture);
        shadow.texture = 0;
    }
}

Model generateAxis(float size)
{
    const float half = size / 2.0f;
    std::vector<float> vertices =
        {
            -half, 0.0f, 0.0f,
            half, 0.0f, 0.0f,
            0.0f, -half, 0.0f,
            0.0f, half, 0.0f,
            0.0f, 0.0f, -half,
            0.0f, 0.0f, half};
    std::vector<float> normals =
        {
            0.0f,1.0f,0.0f,
            0.0f,1.0f,0.0f,
            0.0f,1.0f,0.0f,
            0.0f,1.0f,0.0f,
            0.0f,1.0f,0.0f,
            0.0f,1.0f,0.0f
        };
    std::vector<float> texCoords =
        {
            0.0f, 0.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            0.0f, 1.0f};
    Model result = {vertices, normals, texCoords};
    return result;
}

#endif