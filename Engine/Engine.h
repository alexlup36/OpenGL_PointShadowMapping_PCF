#pragma once

#include <memory>

#include "Shader.h"
#include "Util.h"
#include "Scene.h"
#include "Camera.h"


class Engine
{
public:
	Engine(void);

	int InitializeWindow();
	void SetWindowProperties(const char* windowTitle);

	virtual ~Engine(void);

	void Run();

	void SetUpScene();

	static unsigned int WindowWidth;
	static unsigned int WindowHeight;

private:

	void GetGLData(GLint& major, GLint& minor, bool bPrintExtensions);

	Camera* camera;

	std::unique_ptr<Scene> mainScene;

	GLFWwindow* window;
	GLint iMajor, iMinor;

	void Update(float dt);
	void Draw(float dt);
	Lighting* light;

	float nbFrames;
};

