#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include <string>
#include <vector>

#include <glm/glm.hpp>

struct Model
{
    std::string path;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
};

#endif // MODEL_H_INCLUDED