#include "Camera.h"


//***Default camera***
// Position 0.0f, 0.0f, 0.0f
// Target   0.0f, 0.0f, 1.0f
// Up		0.0f, 1.0f, 0.0f
// NearClip 0.1f
// FarClip  300.0f
Camera::Camera(const char* cameraName)
	: _cameraPosition(glm::vec3(-0.371821f, 4.73271f, 15.5139f)),
	_cameraTarget(0.0f, 0.0f, 0.0f),
	_cameraUp(-0.0001866f, 0.998213f, -0.0597529f),
	_nearClippingPlane(0.1f),
	_farClippingPlane(30000.0f),
	_FOV(45.0f),
	_windowWidth(WINDOW_WIDTH),
	_windowHeight(WINDOW_HEIGHT) 
{
	_horizontalAngle = -PI;// 0.0f;
	_verticalAngle = 0.0f;

	CameraName = new char[strlen(cameraName) + 1];
	strcpy_s(CameraName, strlen(cameraName) + 1, cameraName);

	// Debug
	std::cout<<CameraName<<std::endl;

	SetView();
	SetProjection(_FOV, _windowWidth, _windowHeight, _nearClippingPlane, _farClippingPlane);
}

Camera::Camera(
		glm::vec3& position, 
		glm::vec3& target, 
		glm::vec3& up, 
		GLfloat near, 
		GLfloat far, 
		GLfloat fieldOfView,
		unsigned int windowWidth, 
		unsigned int windowHeight,
		const char* cameraName)
	: _cameraPosition(position),
	_cameraTarget(target),
	_cameraUp(up),
	_nearClippingPlane(near),
	_farClippingPlane(far),
	_FOV(fieldOfView),
	_windowWidth(windowWidth),
	_windowHeight(windowHeight)
{
	_horizontalAngle = 0.0f;
	_verticalAngle = 0.0f;

	CameraName = new char[strlen(cameraName) + 1];
	strcpy_s(CameraName, strlen(cameraName) + 1, cameraName);

	// Debug
	std::cout<<CameraName<<std::endl;

	SetView();
	SetProjection(_FOV, _windowWidth, _windowHeight, _nearClippingPlane, _farClippingPlane);
}

Camera::~Camera(void)
{
	delete CameraName;
}


// Initialize view matrix
void Camera::SetView()
{
	_viewMatrix = glm::lookAt(this->_cameraPosition, this->_cameraTarget, this->_cameraUp);
}
	
// Initialize projection matrix
void Camera::SetProjection(float fov, unsigned int width, unsigned int height, float near, float far)
{
	this->_FOV = fov;
	this->_windowWidth = width;
	this->_windowHeight = height;
	this->_nearClippingPlane = near;
	this->_farClippingPlane = far;

	// Update the projection matrix
	_projectionMatrix = glm::perspective(this->_FOV, (GLfloat)_windowWidth / _windowHeight, _nearClippingPlane, _farClippingPlane);
}

void Camera::SetOrthoProjection(float left, float right, float bottom, float top, float near, float far)
{
	_projectionMatrix = glm::ortho(left, right, bottom, top, near, far);
}

// Getters
glm::mat4 Camera::ProjectionMatrix()
{
	return _projectionMatrix;
}

glm::mat4 Camera::ViewMatrix()
{
	return _viewMatrix;
}

glm::vec3 Camera::GetCameraPosition()
{
	return _cameraPosition;
}

glm::vec3 Camera::GetCameraTarget()
{
	return _cameraTarget;
}
	
glm::vec3 Camera::GetCameraUp()
{
	return _cameraUp;
}

	
// Setters
void Camera::SetPosition(const glm::vec3& pos)
{
	_cameraPosition = pos;
}
	
void Camera::SetTarget(const glm::vec3& targ)
{
	_cameraTarget = targ;
}
	
void Camera:: SetUp(const glm::vec3& up)
{
	_cameraUp = up;
}

void Camera::UpdateMatrices(GLFWwindow* window, float dt)
{
	// Update the mouse cursor position
	glfwGetCursorPos(window, &Input::xMousePos, &Input::yMousePos);
	
	// Update the camera angles
	_horizontalAngle	+= Input::mouseSpeed * dt * float(WINDOW_WIDTH / 2 - Input::xMousePos);
	_verticalAngle		+= Input::mouseSpeed * dt * float(WINDOW_HEIGHT / 2 - Input::yMousePos);

	// Debug - display camera angles
	//std::cout<<_horizontalAngle<<std::endl;
	//std::cout<<_verticalAngle<<std::endl;

	// Calculate the new direction, right and up vectors
	glm::vec3 direction(
		cos(_verticalAngle) * sin(_horizontalAngle),
		sin(_verticalAngle),
		cos(_verticalAngle) * cos(_horizontalAngle));

	glm::vec3 right(
		sin(_horizontalAngle - PI / 2),
		0.0f,
		cos(_horizontalAngle - PI / 2));

	glm::vec3 up(glm::cross(right, direction));

	// Input update
	Input::Update(window, dt, _cameraPosition, direction, right);

	// Debug - print camera position
	//printVec3(_cameraPosition);

	// Update the projection matrix
	//_FOV -= 5 * glfwGetMouseWheel();
	SetProjection(_FOV, _windowWidth, _windowHeight, _nearClippingPlane, _farClippingPlane);

	// Update the view matrix
	_cameraTarget = _cameraPosition + direction;
	_cameraUp = up;
	SetView();
}

void Camera::LoadIndentity()
{
	_projectionMatrix = glm::mat4(1.0f);
	_viewMatrix = glm::mat4(1.0f);
}