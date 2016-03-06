cmake_minimum_required(VERSION 2.8)
project(falling-blocks-cam)

#########################################################
# FIND OpenCV
#########################################################
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
add_definitions(${OpenCV_DEFINITIONS})
if(NOT OpenCV_FOUND)
    message(ERROR " OpenCV not found!")
endif(NOT OpenCV_FOUND)

#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)

file(GLOB_RECURSE SRCFILES src/*.cpp)
add_executable(falling-blocks-cam ${SRCFILES})
add_definitions(-std=c++11)
target_link_libraries(falling-blocks-cam ${OpenCV_LIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
