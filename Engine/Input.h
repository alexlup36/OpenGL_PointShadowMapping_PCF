#pragma once

#include "Util.h"

class Input
{
public:
	Input(void);
	~Input(void);

	// Update input
	static void Update(GLFWwindow* window, float dt, glm::vec3& newPosition, const glm::vec3& direction, const glm::vec3& right);

	// Mouse settings
	static float speed;
	static float mouseSpeed;

	// Mouse position
	static double xMousePos;
	static double yMousePos;
};

