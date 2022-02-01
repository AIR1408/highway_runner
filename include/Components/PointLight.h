#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <memory>
#include <glm/vec3.hpp>

#include <Components/Transform.h>

class ShaderProgram;

class PointLight : public Transform
{
public:
    PointLight(std::shared_ptr<ShaderProgram> shader, glm::vec3 position, glm::vec3 color);

    void translate(glm::vec3 distance) override;

    void use() override;
private:
    glm::vec3 _position, _color;
    int uniformLocations[2];
};

#endif