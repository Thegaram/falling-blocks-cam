#include "Graphics.h"

#include <vector>
#include <exception>
#include <iostream>

#include "../common/SharedState.h"

const char* Graphics::windowName = "Falling Blox";
const char* Graphics::vertexShaderPath = "assets/shaders/StandardShading.vertexshader";
const char* Graphics::fragmentShaderPath = "assets/shaders/StandardShading.fragmentshader";
const char* Graphics::textureDDSPath = "assets/textures/uvmap.DDS";
const char* Graphics::textureBMPPath = "assets/textures/uvtemplate.bmp";
const char* Graphics::fireTexturePathPNG = "assets/textures/uvtemplate3.png";
const char* Graphics::blockTexturePathPNG = "assets/textures/uvtemplate4.png";
const char* Graphics::groundTexturePathPNG = "assets/textures/uvtemplate5.png";
const char* Graphics::cubeModelPath = "assets/models/cube.obj";
const char* Graphics::suzanneModelPath = "assets/models/suzanne.obj";

Graphics::Graphics()
{
    // Initialise GLFW
    if(!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(windowWidth, windowHeight, windowName, nullptr, nullptr);
    if(window == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to open GLFW window");
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLEW");
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, windowWidth/2, windowHeight/2);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders(vertexShaderPath, fragmentShaderPath);
    glUseProgram(programID);

    // Get a handle for our "MVP" uniform
    MatrixID = glGetUniformLocation(programID, "MVP");
    ViewMatrixID = glGetUniformLocation(programID, "V");
    ModelMatrixID = glGetUniformLocation(programID, "M");

    // Load the texture
    BlockTexture = readPNG(blockTexturePathPNG);
    GroundTexture = readPNG(groundTexturePathPNG);
    FireTexture = readPNG(fireTexturePathPNG);

    // Get a handle for our "myTextureSampler" uniform
    TextureID  = glGetUniformLocation(programID, "myTextureSampler");

    // Read our .obj file
    glGenBuffers(1, &vertexbuffer);
    glGenBuffers(1, &uvbuffer);
    glGenBuffers(1, &normalbuffer);

    cubeModel = loadOBJ(cubeModelPath);
    if (!cubeModel)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to load cube model");
    }

    suzanneModel = loadOBJ(suzanneModelPath);
    if (!suzanneModel)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to load suzanne model");
    }

    loadModel(cubeModel);


    // Get a handle for our "LightPosition" uniform
    LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
}

void Graphics::loadModel(std::shared_ptr<Model> model)
{
    if (currentModel == model)
        return;

    std::vector<glm::vec3>& vertices = model->vertices;
    std::vector<glm::vec2>& uvs = model->uvs;
    std::vector<glm::vec3>& normals = model->normals;

    // glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    // glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

    // glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    numVertices = vertices.size();
}

Graphics::~Graphics()
{
    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteBuffers(1, &normalbuffer);
    glDeleteProgram(programID);
    glDeleteTextures(1, &BlockTexture);
    glDeleteTextures(1, &FireTexture);
    glDeleteTextures(1, &GroundTexture);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}

glm::mat4 Graphics::getProjectionMatrix()
{
    return glm::ortho(0.f, 100.f, 0.f, 100.f, -60.f, 60.0f);
}

glm::mat4 Graphics::getViewMatrix()
{
    static SharedState& state = SharedState::getInstance();
    static float eyePosY = 0;

    float newEyePosY = - 4 + 8 * state.getHeadPositionY();
    eyePosY = (15 * eyePosY + newEyePosY) / 16.0;

    // float x = 2 - 4 * ss.getHeadPositionY();

    glm::vec3 cam = glm::vec3( 2, -eyePosY, 20 );
    glm::vec3 mid = glm::vec3( 0, eyePosY, 0 );
    glm::vec3 up = glm::vec3( 0, 1, 0 );

    return glm::lookAt(
        cam, // Camera is here
        mid, // and looks here : at the same position, plus "direction"
        up   // Head is up (set to 0,-1,0 to look upside-down)
    );
}

void Graphics::drawModel(std::shared_ptr<Model> model,
                         glm::vec3 translation,
                         glm::vec3 scaling,
                         Textures texture)
{
    loadModel(model);
    static SharedState& ss = SharedState::getInstance();


    // Compute the MVP matrix from keyboard and mouse input
    glm::mat4 ProjectionMatrix = getProjectionMatrix();
    glm::mat4 ViewMatrix = getViewMatrix();
    glm::mat4 ModelMatrix = glm::mat4(1.0);
    ModelMatrix = glm::translate(ModelMatrix, translation);
    ModelMatrix = glm::scale(ModelMatrix, scaling);

    glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
    glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

    GLuint currentTexture = BlockTexture;
    switch(texture)
    {
        case BLOCK:  currentTexture = BlockTexture;  break;
        case FIRE:   currentTexture = FireTexture;   break;
        case GROUND: currentTexture = GroundTexture; break;
    }

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, currentTexture);
    // Set our "myTextureSampler" sampler to user Texture Unit 0
    glUniform1i(TextureID, 0);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(
        1,                                // attribute
        2,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(
        2,                                // attribute
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // Draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, numVertices);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void Graphics::setLights()
{
    glm::vec3 lightPos = glm::vec3(60, 10, 10);
    glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
}

void Graphics::onLoopStart()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setLights();
}

void Graphics::onLoopEnd()
{
    glfwSwapBuffers(window);
    glfwPollEvents();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        throw std::runtime_error("game escaped");
}

void Graphics::drawCube(glm::vec3 center, float side, Textures texture)
{
    glm::vec3 translation = center;
    float sf = 0.5 * side;
    glm::vec3 scaling = glm::vec3(sf, sf, sf);
    drawModel(cubeModel, translation, scaling, texture);
}


void Graphics::drawSuzanne(glm::vec3 center, float side)
{
    glm::vec3 translation = center;
    float sf = 0.5 * side;
    glm::vec3 scaling = glm::vec3(sf, sf, sf);
    drawModel(suzanneModel, translation, scaling, FIRE);
}