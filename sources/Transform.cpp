#include <Components/Transform.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <ShaderProgram.h>

Transform::Transform(std::shared_ptr<ShaderProgram> shader)
{
    uniformLocation = shader->getUniformLocation("model");
    matrix = glm::mat4(1.0f);
    movement = {0.0, 0.0, 0.0};
};

void Transform::translate(glm::vec3 distance)
{
    matrix = glm::translate(matrix, distance / glm::vec3(matrix[0].x, matrix[1].y, matrix[2].z));
    movement = distance;
}

void Transform::scale(glm::vec3 attitute)
{
    matrix = glm::scale(matrix, attitute);
}

void Transform::rotate(float angleInRadians, glm::vec3 around)
{
    matrix = glm::rotate(matrix, angleInRadians, around);
}

glm::vec3 Transform::getPosition()
{
    return {matrix[3].x, matrix[3].y, matrix[3].z}; 
}

void Transform::use()
{
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &(matrix[0].x));
    movement = {0.0, 0.0, 0.0};
}

void Transform::setCollisionBox(CollisionBox box)
{
    collisionBox = box;
}

bool Transform::checkCollision(std::shared_ptr<Transform> other)
{
    return collisionBox.checkCollision(other->collisionBox, getPosition(), other->getPosition());
}

glm::vec3 Transform::getMovement()
{
    return movement;
}
