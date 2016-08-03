#pragma once
#include "Util.h"
#include "Lighting.h"
#include "Object.h"
#include "Camera.h"
#include "Text2D.h"

#include <vector>
#include <map>

__declspec(align(16)) class Scene
{
public:
	Scene(GLFWwindow* window, const std::string& sceneName);//, GLuint shaderProgram);
	~Scene(void);

	// Initialize code
	void Initialize();

	// Display the scene
	void Display(float dt);
	void Update(float dt);

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

	// *******************************OBJECTS**************************************************
	// Create object
	Object* CreateObject(const std::string& objectName, Object* originalObject, Object* parentObject);
	// Get object by name
	Object* GetObject(const std::string& objectName);
	// Add shader to the list of shaders
	//GLuint AddShader(const char* vertexShaderPath, const char* fragmentShaderPath);
	// Function which does the shader loading
	void LoadShaders();

	// *******************************CAMERAS**************************************************
	// Create camera
	Camera* CreateCamera(const char* camName);
	// Set the active camera
	void SetActiveCamera(Camera* newCamera);
	// Get the active camera
	Camera* GetActiveCamera();
	// Get camera
	Camera* GetCamera(const char* camName);
	Camera* GetCamera(int index);
	// Get the image from the light's position
	void GetImageFromLightPosition(float dt);

	void DrawCubeFace(int iFace);


	// ******************************LIGHTS***************************************************
	void CreateDirectionalLight(glm::vec3 direction);
	void CreatePointLights();

	unsigned short CreateLight(glm::vec3& position);
	void UpdateLight(float dt);

	void SetAmbientLight(glm::vec3& ambientLightColor, unsigned int lightIndex);
	void SetDiffuseLight(glm::vec3& diffuseLight, unsigned int lightIndex);
	void SetSpecularLight(glm::vec3& specularLight, unsigned int lightIndex);

	void SetLightPosition(glm::vec3& newPosition, unsigned int lightIndex);

	void MoveLight(glm::vec3& positionDifference, unsigned int lightIndex);

	// Switch all lights
	void LightSwitch();
	// Switch a specific light source
	void LightSwitch(unsigned int lightIndex);

	GLuint createTexture(int w, int h, bool isDepth);
	GLuint createCubeTexture(int w, int h);

	GLuint createFBO();
	Mesh* createQuad();

	// Name of the scene
	std::string sceneName;

private:
	// Main window reference
	GLFWwindow* _window;

	// List of objects
	std::vector<Object*> _objectList;

	// List of cameras
	std::vector<Camera*> _cameraList;

	// Light handler
	Lighting* _Light;

	// Display object
	void DisplayObjects();
	void DisplayDepthMap();
	void DisplayShadowMap();
	void DisplayFPS(float dt);

	// Use all lights
	void SetLight();

	// Currently active camera
	Camera* _activeCamera;

	// Light status
	bool _isLightOn;

	// Iterators
	std::vector<Object*>::const_iterator _objectListIterator;
	std::vector<Camera*>::const_iterator _cameraListIterator;

	// Framebuffer object
	GLuint FBO;
	// Texture used to render into
	GLuint renderTexture;
	GLuint depthTexture;
	GLuint shadowMapTexture;
	GLuint cubeTexture;

	// Quad
	Mesh* quadToRenderTo;

	// Main shader
	Shader* mainShader;
	// Shader program which draw into a quad
	Shader* quadShader;

	Shader* simpleShader;

	Shader* shadowShader;
	Shader* cubeShader;

	GLuint quad_vertexbuffer;

	// Camera
	glm::vec3 cameraPosition;
	glm::vec3 cameraTarget;
	glm::vec3 cameraUp;

	// Light's modelviewprojmatrix
	glm::mat4 shadowMatrix;

	// Shadow map dimensions
	unsigned int shadowMapWidth;
	unsigned int shadowMapHeight;

	GLfloat angle;

	// Point lights
	std::vector<PointLight> pointLights;

	glm::mat4 biasMatrix;

	unsigned int CUBE_TEXTURE_SIZE;

	//glm::vec3 lightCameraSpace0;
	//glm::mat4 viewMatTranslation[3];

	bool KeyPress(int key);

	char keyOnce[GLFW_KEY_LAST + 1];

	float mFPS;
	float mMilisecondsPerFrame;
};