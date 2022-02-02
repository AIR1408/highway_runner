#ifndef THIRD_PERSON_CAMERA_H
#define THIRD_PERSON_CAMERA_H

#include <Components/Camera.h>
#include <Container.h>
#include <Components/Transform.h>
#include <Components/CollisionBox/CollisionBox.h>

class ThirdPersonCamera : public Camera
{
public:
    ThirdPersonCamera(GLFWwindow* window, std::shared_ptr<ShaderProgram> shader,
                glm::vec3 position, Container car);

    void setCollisionBox(CollisionBox box);
    glm::vec3 getPosition() override;
    std::shared_ptr<Transform> getTransform();

    void render(float deltaTime);
private:
    std::shared_ptr<Transform> object_matrix;

    Container _container;
};

#endif