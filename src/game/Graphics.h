#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../utils/shader.hpp"
#include "../utils/texture.hpp"
#include "../utils/objloader.hpp"
#include "../utils/Model.h"

class Graphics
{
private:
    GLFWwindow* window;

    GLuint VertexArrayID;
    GLuint programID;

    GLuint MatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;

    GLuint BlockTexture;
    GLuint FireTexture;
    GLuint GroundTexture;
    GLuint TextureID;

    GLuint vertexbuffer;
    GLuint uvbuffer;
    GLuint normalbuffer;
    GLuint numVertices;

    GLuint LightID;

    std::shared_ptr<Model> cubeModel    = nullptr;
    std::shared_ptr<Model> suzanneModel = nullptr;
    std::shared_ptr<Model> surfaceModel = nullptr;
    std::shared_ptr<Model> currentModel = nullptr;

private:
    static const char* windowName;

    static const char* vertexShaderPath;
    static const char* fragmentShaderPath;

    static const char* textureDDSPath;
    static const char* textureBMPPath;
    static const char* fireTexturePathPNG;
    static const char* blockTexturePathPNG;
    static const char* groundTexturePathPNG;

    static const char* cubeModelPath;
    static const char* suzanneModelPath;

    static const int windowWidth = 800;
    static const int windowHeight = 800;

public:
    enum Textures
    {
        GROUND,
        FIRE,
        BLOCK
    };

private:
    Graphics();

    void loadModel(std::shared_ptr<Model> model);
    void drawModel(std::shared_ptr<Model> model, glm::vec3 translation, glm::vec3 scaling, Textures texture);
    void setLights();

    glm::mat4 getProjectionMatrix();
    glm::mat4 getViewMatrix();

public:
    static Graphics& getInstance()
    {
        static Graphics instance;
        return instance;
    }

    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;

    ~Graphics();

    void onLoopStart();
    void onLoopEnd();

    void drawCube(glm::vec3 center, float side = 1, Textures texture = BLOCK);
    void drawSuzanne(glm::vec3 center, float side = 1);
};

#endif // GRAPHICS_H_INCLUDED