#include <iostream>
#include <chrono>
#include <thread>

#include <GL/glut.h>
#include <OpenGL/gl3.h>

#include "./game.h"
#include "../common/SharedState.h"
#include "../common/Logger.h"

#include "./Controller.h"

constexpr int WINDOW_WIDTH = 480;
constexpr int WINDOW_HEIGHT = 600;

using namespace std;

SharedState& state = SharedState::getInstance();
Logger& logger = Logger::getInstance();

Controller controller;

long elapsedTime;

void draw()
{
    static float eyePosY = 0;

    // Black background
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set camera
    float headPosY = state.getHeadPositionY();
    float newEyePosY = -40 + 80 * (1 - headPosY);
    eyePosY = (7 * eyePosY + newEyePosY) / 8.0;
    float eyePosX = 10;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 0.0, eyePosX, eyePosY, -100.0, 0.0, 1.0, 0.0);

    // draw ground
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (eyePosY > 0)
    {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_QUADS);
            glColor3f(0.1, 0.2, 1.0);
            glVertex3f(-5.0f, -1.0f, -5.0f);
            glVertex3f(+5.0f, -1.0f, -5.0f);
            glVertex3f(+5.0f, -1.0f, +5.0f);
            glVertex3f(-5.0f, -1.0f, +5.0f);
        glEnd();
    }

    // draw game objects
    long newTime = glutGet(GLUT_ELAPSED_TIME);
    long dt = newTime - elapsedTime;
    elapsedTime = newTime;

    controller.mainLoop(dt);


    glutSwapBuffers();
    glutPostRedisplay();
}

void resize(int width, int height) {
    glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void initGraphics()
{
    int argc = 1;
    char *argv[1] = {(char*)"Dummy"};

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowPosition(50, 25);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Hello OpenGL");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    elapsedTime = glutGet(GLUT_ELAPSED_TIME);

    glutDisplayFunc(draw);
    glutReshapeFunc(resize);
}

void startGraphicsLoop()
{
    glutMainLoop();
}

void gameLoop()
{
    initGraphics();
    startGraphicsLoop();
}