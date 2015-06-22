#include "Input.h"

// Initialization

// Mouse settings
GLfloat Input::speed		= 4.0f;
GLfloat Input::mouseSpeed	= 0.3f;

// Mouse position
double Input::xMousePos		= WINDOW_WIDTH / 2;
double Input::yMousePos		= WINDOW_HEIGHT / 2;

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
	glfwSetCursorPos(window, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);

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