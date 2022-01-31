#include <Components/PointLight.h> 
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <ShaderProgram.h>

PointLight::PointLight(std::shared_ptr<ShaderProgram> shader, glm::vec3 position, glm::vec3 color) : 
    _position(position), _color(color), Transform(shader)
{
    uniformLocations[0] = shader->getUniformLocation("light.position");
    uniformLocations[1] = shader->getUniformLocation("light.color");
    matrix = glm::translate(matrix, _position);
}

void PointLight::translate(glm::vec3 distance)
{
    _position += distance;
    matrix = glm::translate(matrix, distance);
}

void PointLight::use()
{
    glUniform3f(uniformLocations[0], _position.x, _position.y, _position.z);
    glUniform3f(uniformLocations[1], _color.x, _color.y, _color.z);
    Transform::use();
}