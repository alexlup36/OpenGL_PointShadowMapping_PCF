#include "Engine.h"

Engine::Engine(void)
{
}

Engine::~Engine(void)
{
}

void Engine::Run()
{
	do
	{
		static double lastTime = glfwGetTime();

		double currentTime = glfwGetTime();
		float dt = float(currentTime - lastTime);

		// Processing
		this->Update(dt);
		this->Draw(dt);

		lastTime = currentTime;
	} 
	while ( glfwGetKey( window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		glfwGetWindowAttrib(window, GLFW_VISIBLE));

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

void Engine::SetUpScene()
{	
	glGetError();

	// Create a scene
	mainScene = std::make_unique<Scene>(window, "MainScene");//, programID);

	mainScene->CreateCamera("FPSCamera");
	// Create the light camera
	mainScene->CreateCamera("DirectionalLightCamera");
	// Create a generic point light camera
	mainScene->CreateCamera("PointLight");
	
	// Create an object
	mainScene->CreateObject("StaticScene", NULL, NULL);
	mainScene->GetObject("StaticScene")->LoadObj(".\\Models\\mymodelsimple.obj");

	mainScene->CreateObject("Torus", NULL, NULL);
	mainScene->GetObject("Torus")->LoadObj(".\\Models\\torus.obj");

	mainScene->CreateObject("LightObject0", NULL, NULL);
	mainScene->GetObject("LightObject0")->LoadObj(".\\Models\\light.obj");

	/*mainScene->CreateObject("LightObject1", NULL, NULL);
	mainScene->GetObject("LightObject1")->LoadObj(".\\Models\\light.obj");

	mainScene->CreateObject("LightObject2", NULL, NULL);
	mainScene->GetObject("LightObject2")->LoadObj(".\\Models\\light.obj");*/

	mainScene->GetObject("StaticScene")->LoadIntoVB();
	mainScene->GetObject("Torus")->LoadIntoVB();
	mainScene->GetObject("LightObject0")->LoadIntoVB();
	//mainScene->GetObject("LightObject1")->LoadIntoVB();
	//mainScene->GetObject("LightObject2")->LoadIntoVB();
	
	// Run the initialize code (lighting)
	int i = glGetError();
	if (i != 0)
	{
		std::cout<<"Error engine init"<<i<<std::endl;
	}
	mainScene->Initialize();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}


int Engine::InitializeWindow()
{
	// Initialize GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return 0;
	}

	// Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "GLSL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	// Get GL data
	GetGLData(iMajor, iMinor, false);

	// Use 4.4 core profile and forward compatibility
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, iMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, iMinor);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) 
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return 0;
	}

	return 1;
}

void Engine::SetWindowProperties(const char* windowTitle)
{
	glfwSetWindowTitle(window, windowTitle);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);

	glEnable(GL_CULL_FACE);
	//glDisable(GL_CULL_FACE);

	// Hide the mouse cursor
	glfwSetInputMode(window, GLFW_CURSOR, GL_FALSE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
}


void Engine::Update(float dt)
{
	//mainScene->EnableShader();

	// Update the camera
	mainScene->GetActiveCamera()->UpdateMatrices(window, dt);

	mainScene->Update(dt);
}

void Engine::Draw(float dt)
{
	// Display the main scene
	mainScene->Display(dt);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Engine::GetGLData(GLint& major, GLint& minor, bool bPrintExtensions)
{
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	printf("GL Vendor : %s\n", vendor);
	printf("GL Renderer : %s\n", renderer);
	printf("GL Version (string) : %s\n", version);
	printf("GL Version (integer) : %d.%d\n", major, minor);
	printf("GLSL Version : %s\n", glslVersion);

	GLint nExtensions;
	glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);
	if (bPrintExtensions)
	{
		for (int i = 0; i < nExtensions; i++)
		{
			printf("%s\n", glGetStringi(GL_EXTENSIONS, i));
		}
	}
}