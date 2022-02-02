#ifndef CAMERA_H
#define CAMERA_H

#include <memory>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <GLFW/glfw3.h>

#include <Components/Component.h>

class ShaderProgram;

class Camera : public Component
{
public:
	Camera(GLFWwindow* window, std::shared_ptr<ShaderProgram> shader, glm::vec3 position, glm::vec3 lookDirection);    

	void switchMouseBlock();
	void switchKeyboardBlock();
	void switchKeyBlock(int glfw_key);

	virtual glm::vec3 getPosition();
	glm::mat4 getViewMatrix();

	void use() override;

protected:
	virtual void CameraKeyboard(float deltaTime);
	virtual void CameraMouse();

	bool isFirstIteration, isMouseBlocked,
		isKeyboardBlocked, isKeyBlocked[4];

	glm::vec3 position, front, up;

	float speed, mouseSensitivity;
	float currentTime, lastTime;
	float yaw, pitch;
	glm::dvec2 currentCursorPosition;
	glm::dvec2 lastCursorPosition;

    int _matrixUniformLocation, _positionUniformLocation;
    glm::mat4 View;

    GLFWwindow* p_window;
};

#endif // !CAMERA_H
