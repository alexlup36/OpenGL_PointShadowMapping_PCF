#include "Input.h"

// Initialization

// Mouse settings
GLfloat Input::speed		= 4.0f;
GLfloat Input::mouseSpeed	= 0.3f;

// Mouse position
double Input::xMousePos		= 1024 / 2;
double Input::yMousePos		= 768 / 2;

Input::Input(void)
{
}


Input::~Input(void)
{
}

void Input::Update(GLFWwindow* window, float dt, glm::vec3& newPosition, const glm::vec3& direction, const glm::vec3& right)
{
	glfwGetCursorPos(window, &xMousePos, &yMousePos);

	// Set mouse position to the center of the screen
	glfwSetCursorPos(window, 1024.0f / 2.0f, 768.0f / 2.0f);

	// Move forward
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		newPosition += direction * dt * Input::speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		newPosition -= direction * dt * Input::speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
		newPosition += right * dt * Input::speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
		newPosition -= right * dt * Input::speed;
	}
}