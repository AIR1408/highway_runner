#ifndef EXTRA_H
#define EXTRA_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace ext 
{

struct Vertex 
{
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec3 normal;
};

}

#endif // !EXTRA_H
