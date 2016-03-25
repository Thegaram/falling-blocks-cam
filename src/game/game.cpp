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
    // Black background
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // float x = state.getHeadPositionX();

    // glBegin(GL_TRIANGLES);
    //     glColor3f(0.1, 0.2, 0.3);
    //     glVertex2f(x - 1, -1);
    //     glVertex2f(1, -1);
    //     glVertex2f(-1, 1);
    // glEnd();

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
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

    glutInitWindowPosition(50, 25);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Hello OpenGL");

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