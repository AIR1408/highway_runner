#include <glad/glad.h>
#include <Components/Camera.h>
#include <glm/gtc/matrix_transform.hpp>
#include <ShaderProgram.h>

Camera::Camera(GLFWwindow* window, std::shared_ptr<ShaderProgram> shader, glm::vec3 position, glm::vec3 lookAt) : 
                                                        speed(0), mouseSensitivity(0.005),
                                                        currentTime(0), lastTime(0),
                                                        yaw(0), pitch(0), p_window(window),
                                                        isFirstIteration(true)
{
	this->position = position;
	front = glm::normalize(lookAt - position);
	up = glm::vec3(0.0f, 1.0f, 0.0f);

    yaw   = acos(front.x / sqrt(front.x * front.x + front.z * front.z)),
    pitch = acos(sqrt(front.x * front.x  + front.z * front.z)) * glm::sign(front.y);
    //yaw = asin(front.x) * 3.14 / 180, pitch = asin(front.y) * 3.14 / 180;
    speed = 2.0;

	View = glm::lookAt(position, position + front, up);
    _matrixUniformLocation = shader->getUniformLocation("view");
    _positionUniformLocation = shader->getUniformLocation("viewPos");

    isMouseBlocked = isKeyboardBlocked = false;
    for (bool& t : isKeyBlocked)
        t = false;
}

void Camera::switchMouseBlock()
{
    isMouseBlocked = !isMouseBlocked;
}

void Camera::switchKeyboardBlock()
{
    isKeyboardBlocked = !isKeyboardBlocked;
}

void Camera::switchKeyBlock(int glfw_key)
{
    if (glfw_key == GLFW_KEY_W) isKeyBlocked[0] = !isKeyBlocked[0];
    else if (glfw_key == GLFW_KEY_S) isKeyBlocked[1] = !isKeyBlocked[1];
    else if (glfw_key == GLFW_KEY_A) isKeyBlocked[2] = !isKeyBlocked[2];
    else if (glfw_key == GLFW_KEY_D) isKeyBlocked[3] = !isKeyBlocked[3];
}

glm::vec3 Camera::getPosition()
{
    return position;
}

glm::mat4 Camera::getViewMatrix()
{
    return View;
}

void Camera::use()
{
    
    currentTime = glfwGetTime();
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    if (!isKeyboardBlocked)
        CameraKeyboard(deltaTime);
    if (!isMouseBlocked)
        CameraMouse();

    View = glm::lookAt(position, position + front, up);
    glUniformMatrix4fv(_matrixUniformLocation, 1, GL_FALSE, &(View[0].x));
    glUniform3f(_positionUniformLocation, position.x, position.y, position.z);
}

void Camera::CameraKeyboard(float deltaTime)
{
    if (glfwGetKey(p_window, GLFW_KEY_W) == GLFW_PRESS && !isKeyBlocked[0])
        position += speed * front * deltaTime;
    if (glfwGetKey(p_window, GLFW_KEY_S) == GLFW_PRESS && !isKeyBlocked[1])
        position -= speed * front * deltaTime;
    if (glfwGetKey(p_window, GLFW_KEY_A) == GLFW_PRESS && !isKeyBlocked[2])
        position -= glm::normalize(glm::cross(front, up)) * speed * deltaTime;
    if (glfwGetKey(p_window, GLFW_KEY_D) == GLFW_PRESS && !isKeyBlocked[3])
        position += glm::normalize(glm::cross(front, up)) * speed * deltaTime;
}

void Camera::CameraMouse()
{
    lastCursorPosition = currentCursorPosition;
    glfwGetCursorPos(p_window,
                    &currentCursorPosition.x, 
                    &currentCursorPosition.y);

    if (isFirstIteration)
        lastCursorPosition = currentCursorPosition,
        isFirstIteration = false;

    glm::dvec2 cursorOffset = currentCursorPosition - lastCursorPosition;

    cursorOffset *= mouseSensitivity;

    yaw   +=  static_cast<float>(cursorOffset.x);
    pitch += -static_cast<float>(cursorOffset.y);

    if (pitch > 1.55f)
        pitch = 1.55f;
    if (pitch < -1.55f)
        pitch = -1.55f;

    front.x = cos(yaw) * cos(pitch);
    front.y = sin(pitch);
    front.z = sin(yaw) * cos(pitch);
}

