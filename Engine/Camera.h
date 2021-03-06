#pragma once

#include "Util.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Input.h"

class Camera
{
public:
	Camera(const char* cameraName);
	Camera(
		glm::vec3& position, 
		glm::vec3& target, 
		glm::vec3& up, 
		GLfloat near, 
		GLfloat far, 
		GLfloat fieldOfView,
		unsigned int windowWidth, 
		unsigned int windowHeight,
		const char* cameraName);

	// ---------------------------------------------------------------------------

	inline void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

		inline void operator delete(void* p)
	{
		_mm_free(p);
	}

	// ---------------------------------------------------------------------------

	// Initialize view matrix
	void SetView();
	// Initialize projection matrix
	void SetProjection(float fov, unsigned int width, unsigned int height, float near, float far);
	void SetOrthoProjection(float left, float right, float bottom, float top, float near, float far);

	// Getters
	glm::mat4 ProjectionMatrix();
	glm::mat4 ViewMatrix();
	glm::vec3 GetCameraPosition();
	glm::vec3 GetCameraTarget();
	glm::vec3 GetCameraUp();

	// Setters
	void SetPosition(const glm::vec3& pos);
	void SetTarget(const glm::vec3& targ);
	void SetUp(const glm::vec3& up);

	void UpdateMatrices(GLFWwindow* window, float dt);

	void LoadIndentity();

	virtual ~Camera(void);

	// Camera name
	char* CameraName;

private:
	// Camera information
	GLfloat _nearClippingPlane;
	GLfloat _farClippingPlane;
	GLfloat _FOV;

	glm::vec3 _cameraPosition;
	glm::vec3 _cameraTarget;
	glm::vec3 _cameraUp;

	// Window dimension
	unsigned int _windowWidth;
	unsigned int _windowHeight;

	// Transformation matrices
	glm::mat4 _projectionMatrix;
	glm::mat4 _viewMatrix;

	// Camera orientation
	GLfloat _horizontalAngle;
	GLfloat _verticalAngle;

	// Time difference between two update calls
	GLfloat deltaTime;
};

