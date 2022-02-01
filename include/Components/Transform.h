#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <memory>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <Components/Component.h>
#include "CollisionBox/CollisionBox.h"

class ShaderProgram;

class Transform : public Component
{
public:
    Transform(std::shared_ptr<ShaderProgram> shader);

    virtual void translate(glm::vec3 distance);
    virtual void scale(glm::vec3 attitute);
    virtual void rotate(float angleInRadians, glm::vec3 around);

    glm::vec3 getPosition();

    void use() override;

    void setCollisionBox(CollisionBox box);
    bool checkCollision(std::shared_ptr<Transform> other);
    glm::vec3 getMovement();

    
protected:
    glm::mat4 matrix;
    glm::vec3 movement;
private:
    int uniformLocation;

    CollisionBox collisionBox;
};

#endif