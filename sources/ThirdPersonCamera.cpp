#include <ThirdPersonCamera.h>

ThirdPersonCamera::ThirdPersonCamera(
    GLFWwindow* window, std::shared_ptr<ShaderProgram> shader,
    glm::vec3 position, Container container) : Camera(window, shader, 
    position + glm::vec3(0.0, 3.5, -4.5), position + glm::vec3(0.0, 0.0, 10.0))
{
    _container = container;
    p_window = window;
    object_matrix = std::make_shared<Transform>(shader);
    object_matrix->translate(position);
    speed = 3.75;
    switchMouseBlock();
    switchKeyBlock(GLFW_KEY_W);
    switchKeyBlock(GLFW_KEY_S);
}

void ThirdPersonCamera::render(float deltaTime)
{
    object_matrix->translate(position - object_matrix->getPosition() - glm::vec3(0.0, 3.5, -4.5));

    use();
    object_matrix->use();
    _container.render();
}

glm::vec3 ThirdPersonCamera::getPosition()
{
    return object_matrix->getPosition();
}

std::shared_ptr<Transform> ThirdPersonCamera::getTransform()
{
    return object_matrix;
}

void ThirdPersonCamera::setCollisionBox(CollisionBox box)
{
    object_matrix->setCollisionBox(box);
}
