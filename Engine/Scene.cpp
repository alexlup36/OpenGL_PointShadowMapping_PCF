#include "Scene.h"
#include <fstream>

Scene::Scene(GLFWwindow* window, const std::string& sceneName)//, GLuint shaderProgram)
	: sceneName(sceneName),
	_objectList(0),
	_cameraList(0),
	_objectListIterator(),
	_cameraListIterator(),
	_activeCamera(NULL),
	_isLightOn(true),
	_window(window)
{
	std::cout<<"Scene creation: "<<sceneName<<std::endl;

	// Init shadow map dimensions
	shadowMapWidth = 1024 * 1/4;
	shadowMapHeight = 700 * 1/4;

	// Rotation angle - default
	angle = 0.0f;

	// Init bias matrix
	biasMatrix[0][0] = 0.5f; biasMatrix[0][1] = 0.0f; biasMatrix[0][2] = 0.0f; biasMatrix[0][3] = 0.0f;
	biasMatrix[1][0] = 0.0f; biasMatrix[1][1] = 0.5f; biasMatrix[1][2] = 0.0f; biasMatrix[1][3] = 0.0f;
	biasMatrix[2][0] = 0.0f; biasMatrix[2][1] = 0.0f; biasMatrix[2][2] = 0.5f; biasMatrix[2][3] = 0.0f;
	biasMatrix[3][0] = 0.5f; biasMatrix[3][1] = 0.5f; biasMatrix[3][2] = 0.5f; biasMatrix[3][3] = 1.0f;

	// Store the directional light properties
	cameraPosition	= glm::vec3(-4.426f, 7.615f, 10.492f);
	cameraTarget	= glm::vec3(-4.058f, 7.231f, 9.645f);
	cameraUp		= glm::vec3(0.153f, 0.923f, -0.352f);

	// Set the cube texture size
	CUBE_TEXTURE_SIZE = 256;
}


Scene::~Scene(void)
{
	// Delete the cameras
	while (!_cameraList.empty())
	{
		delete _cameraList.back();
		_cameraList.pop_back();
	}

	// Delete the lighting handler
	delete _Light;

	// Delete the objects in the scene
	while (!_objectList.empty())
	{
		delete _objectList.back();
		_objectList.pop_back();
	}
}

void Scene::Initialize()
{
	glGetError();

	LoadShaders();

	// Error checking
	int i = glGetError();
	if (i != 0)
	{
		std::cout<<"Error at shader loading: "<<i<<std::endl;
	}
	
	// Create the framebuffer
	createFBO();

	// Create a quad
	quadShader->Enable();
	quadToRenderTo = createQuad();
	quadToRenderTo->LoadIntoVB();
	quadShader->Disable();

	// Error checking
	

	// Enable shadow shader for light setup
	shadowShader->Enable();
		_Light = new Lighting(shadowShader->ProgramID());

		_Light->SetMode(0);
		// Error checking
		
		// Create a directional light
		CreateDirectionalLight(glm::vec3(2.0f, -2.0f, -2.0f));
		// Create point lights
		CreatePointLights();

		i = glGetError();
		if (i != 0)
		{
			std::cout<<"Error Lighting: "<<i<<std::endl;
		}
		
	shadowShader->Disable();

	// Default - translate the light sources up
	GetObject("LightObject0")->Translate(0.0f, 4.0f, 0.0f);

	GetObject("Torus")->Translate(0.0f, 2.0f, 0.0f);
	
	GetObject("LightObject0")->Scale(0.5f, 0.5f, 0.5f);
	GetObject("LightObject0")->Translate(3.0f, sin(angle), 0.0f);

	GetObject("LightObject1")->Translate(4.0f, 2.6f, 0.0f);
	GetObject("LightObject1")->Scale(0.5f, 0.5f, 0.5f);

	GetObject("LightObject2")->Translate(2.0f, 6.0f, -1.0f);
	GetObject("LightObject2")->Scale(0.5f, 0.5f, 0.5f);
	

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	Text2D::InitText2D("..\\Textures\\Holstein.tga");
}

// Display the scene
void Scene::Display(float dt)
{
	// Reset the error
	glGetError();

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);	
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Update the lights
	UpdateLight(dt);
	
	// Create the shadow map
	//GetImageFromLightPosition(dt);

	// Create the cube faces
	for (int i = 0; i < 6; i++)
	{
		DrawCubeFace(i);
	}
	
	shadowShader->Enable();
		// Set the light properties 
		if (_isLightOn)
		{
			// Update the eye position
			_Light->SetEyeWorldPos(GetCamera(0)->GetCameraPosition());
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);

		//Update uniform variables for shadow shader
		glUniform1i(glGetUniformLocation(shadowShader->ProgramID(), "cubeDepthMap"), 0);

		glViewport(0, 0, 1024, 768);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		// Display objects
		DisplayObjects();
	shadowShader->Disable();

	DisplayFPS(dt);
}

	

void Scene::Update(float dt)
{
	// Update angle
	angle += 0.01f;

	// Torus update
	if (glfwGetKey(_window, GLFW_KEY_D))
	{
		bool draw = GetObject("Torus")->IsDrawing();
		GetObject("Torus")->SetDraw(!draw);
	}
	if (glfwGetKey(_window, GLFW_KEY_A))
	{
		bool animate = GetObject("Torus")->IsAnimating();
		GetObject("Torus")->SetAnimate(!animate);
	}
	if (glfwGetKey(_window, GLFW_KEY_P))
	{
		bool animate = GetObject("LightObject0")->IsAnimating();
		GetObject("LightObject0")->SetAnimate(!animate);
	}

	if (GetObject("Torus")->IsAnimating())
	{
		GetObject("Torus")->Translate(0.0f, 2.0f, 0.0f);
		GetObject("Torus")->Rotate(0.0f, 0.0f, 1.0f, 0.3f);
		GetObject("Torus")->Translate(0.0f, -2.0f, 0.0f);
	}

	// Update mesh objects which represent the point light sources
	// Update light 0 object
	if (GetObject("LightObject0")->IsAnimating())
	{
		GetObject("LightObject0")->Translate(0.04f, 0.00f, 0.02f);
		GetObject("LightObject0")->Rotate(0.0f, 1.0f, 5 * sin(angle), 0.15f);
	}
	else
	{
		if (glfwGetKey(_window, GLFW_KEY_L) == GLFW_PRESS)
		{
			GetObject("LightObject0")->Translate(dt * 2.5f, 0.0f, 0.0f);
		}
		if (glfwGetKey(_window, GLFW_KEY_J) == GLFW_PRESS)
		{
			GetObject("LightObject0")->Translate(-dt * 2.5f, 0.0f, 0.0f);
		}
		if (glfwGetKey(_window, GLFW_KEY_I) == GLFW_PRESS)
		{
			GetObject("LightObject0")->Translate(0.0f, 0.0f, -dt * 2.5f);
		}
		if (glfwGetKey(_window, GLFW_KEY_K) == GLFW_PRESS)
		{
			GetObject("LightObject0")->Translate(0.0f, 0.0f, dt * 2.5f);
		}
		if (glfwGetKey(_window, GLFW_KEY_U) == GLFW_PRESS)
		{
			GetObject("LightObject0")->Translate(0.0f, dt * 2.5f, 0.0f);
		}
		if (glfwGetKey(_window, GLFW_KEY_O) == GLFW_PRESS)
		{
			GetObject("LightObject0")->Translate(0.0f, -dt * 2.5f, 0.0f);
		}
	}

	// Update the lighting mode
	if (glfwGetKey(_window, GLFW_KEY_Q))
	{
		_Light->SetMode(0);
	}
	if (glfwGetKey(_window, GLFW_KEY_W))
	{
		_Light->SetMode(1);
	}
	if (glfwGetKey(_window, GLFW_KEY_E))
	{
		_Light->SetMode(2);
	}
}


// *******************************OBJECTS**************************************************
// Create object
Object* Scene::CreateObject(const std::string& objectName, Object* originalObject, Object* parentObject)
{
	std::cout<<"Object creation: "<<objectName<<std::endl;

	// New temp object
	Object* newObject;

	// If the original object is not defined
	if (!originalObject)
	{
		newObject = new Object(objectName);
	}
	else
	{
		newObject = new Object(*originalObject, objectName);
	}

	// If the parent object is defined add the new object to its child list
	if (parentObject)
	{
		// Add the new object to the parent's children list
		parentObject->AddChild(newObject);
		// Set the parent of the object
		newObject->SetParent(parentObject);
	}
	else // If the object is not a child, add it to the list of objects in the scene
	{
		std::cout<<"Object "<<newObject->name << " added to the list of object"<< std::endl;
		// Add the new created object to the list of object in the scene
		_objectList.push_back(newObject);
	}

	return newObject;
}

// Get object by name
Object* Scene::GetObject(const std::string& objectName)
{
	if (objectName == "")
	{
		return NULL;
	}

	for (unsigned int i = 0; i < _objectList.size(); i++)
	{
		if (objectName == _objectList[i]->name)
		{
			return _objectList[i];
		}
	}

	return NULL;
}

// Display object
void Scene::DisplayObjects()
{
	_objectListIterator = _objectList.begin();
	
	
	while (_objectListIterator != _objectList.end())
	{
		// Compute the matrices
		glm::mat4 MVP = GetCamera(0)->ProjectionMatrix() * GetCamera(0)->ViewMatrix() * (*_objectListIterator)->GetModel();
		glm::mat4 World = (*_objectListIterator)->GetModel();
		glm::mat4 ModelView = GetCamera(0)->ViewMatrix() * (*_objectListIterator)->GetModel();
		
		// Set uniforms
		_Light->SetWorld((*_objectListIterator)->GetModel());
		glUniformMatrix4fv(glGetUniformLocation(shadowShader->ProgramID(), "MVP"), 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shadowShader->ProgramID(), "World"), 1, GL_FALSE, &World[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shadowShader->ProgramID(), "ModelView"), 1, GL_FALSE, &ModelView[0][0]);
		
		// Render objects
		(*_objectListIterator)->Display();

		_objectListIterator++;
	}
}

void Scene::DisplayDepthMap()
{
	_objectListIterator = _objectList.begin();
	
	
	while (_objectListIterator != _objectList.end())
	{
		// Compute the MVP matrix for each object using the 
		glm::mat4 MVPDepthLight = GetCamera("PointLight")->ProjectionMatrix() * GetCamera("PointLight")->ViewMatrix() * (*_objectListIterator)->GetModel();

		// Simple shader
		glUniformMatrix4fv(glGetUniformLocation(simpleShader->ProgramID(), "MVPDepthLight"), 1, GL_FALSE, &MVPDepthLight[0][0]);

		// Display object
		(*_objectListIterator)->Display();

		_objectListIterator++;
	}
}

// *******************************CAMERAS**************************************************
// Create camera
Camera* Scene::CreateCamera(const char* camName)
{
	// Create a new default camera
	Camera* newCamera = new Camera(camName);

	// Set the projection matrix for the camera
	newCamera->SetProjection(45.0f, 1024, 768, 0.1f, 30000.0f);

	// If no active camera set it to the new created camera
	if (!_activeCamera)
	{
		_activeCamera = newCamera;
	}

	// Add the new created camera to the camera list
	_cameraList.push_back(newCamera);

	// Camera created
	std::cout<<"Camera created"<<std::endl;

	// Return the newly created camera
	return newCamera;
}

// Set the active camera
void Scene::SetActiveCamera(Camera* newCamera)
{
	_activeCamera = newCamera;
}

// Get the active camera
Camera* Scene::GetActiveCamera()
{
	return _activeCamera;
}

Camera* Scene::GetCamera(const char* camName)
{
	// Iterator for the list of cameras
	std::vector<Camera*>::const_iterator camIterator = _cameraList.begin();

	while (camIterator != _cameraList.end())
	{
		if (strcmp((*camIterator)->CameraName, camName) == 0)
		{
			return *camIterator;
		}

		camIterator++;
	}

	return NULL;
}

Camera* Scene::GetCamera(int index)
{
	return _cameraList[index];
}

// 0 - Positive X;	1 - Negative X;
// 2 - Positive Y;	3 - Negative Y;
// 4 - Positive Z;	5 - Negative Z;
void Scene::DrawCubeFace(int iFace)
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	// Attach the cube map face to the framebuffer
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + iFace, cubeTexture, 0);

	// Set the viewport the same size as the cube texture
	glViewport(0, 0, CUBE_TEXTURE_SIZE, CUBE_TEXTURE_SIZE);
 
	// Check the framebuffer
	GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE)
			printf("Status error: %08x\n", status);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
		
	//glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	// For each cube face go through each point light
	for (size_t i = 0; i < pointLights.size(); i++)
	{
		// Update the camera position - to match the current point light

		// Go to the light's position
		GetCamera("PointLight")->SetPosition(pointLights[i].Position);

		// Depending on the face, set the camera target
		// Set the camera up vector
		switch (iFace)
		{
		case 0:
			GetCamera("PointLight")->SetTarget(pointLights[i].Position + glm::vec3(1.0f, 0.0f, 0.0f));
			GetCamera("PointLight")->SetUp(glm::vec3(0,-1,0));
			break;
		case 1:
			GetCamera("PointLight")->SetTarget(pointLights[i].Position - glm::vec3(1.0f, 0.0f, 0.0f));
			GetCamera("PointLight")->SetUp(glm::vec3(0,-1,0));
			break;
		case 2:
			GetCamera("PointLight")->SetTarget(pointLights[i].Position + glm::vec3(0.0f, 1.0f, 0.0f));
			GetCamera("PointLight")->SetUp(glm::vec3(0,0,1));
			break;
		case 3:
			GetCamera("PointLight")->SetTarget(pointLights[i].Position - glm::vec3(0.0f, 1.0f, 0.0f));	
			GetCamera("PointLight")->SetUp(glm::vec3(0,0,-1));
			break;
		case 4:
			GetCamera("PointLight")->SetTarget(pointLights[i].Position + glm::vec3(0.0f, 0.0f, 1.0f));
			GetCamera("PointLight")->SetUp(glm::vec3(0,-1,0));
			break;
		case 5:
			GetCamera("PointLight")->SetTarget(pointLights[i].Position - glm::vec3(0.0f, 0.0f, 1.0f));
			GetCamera("PointLight")->SetUp(glm::vec3(0,-1,0));
			break;
		}

		// Update the view matrix for the point light camera
		GetCamera("PointLight")->SetView();
		// Set the projection matrix for the camera
		GetCamera("PointLight")->SetProjection(90.0f, CUBE_TEXTURE_SIZE, CUBE_TEXTURE_SIZE, 1.0f, 1000.0f);
		// Set the camera as active
		SetActiveCamera(GetCamera("PointLight"));

		simpleShader->Enable();	
			DisplayDepthMap();
		simpleShader->Disable();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_CULL_FACE);
	SetActiveCamera(GetCamera(0));
	glViewport(0, 0, 1024, 700);
}

// Get the image from the light's position
void Scene::GetImageFromLightPosition(float dt)
{
	// Go to the light's position
	GetCamera("DirectionalLightCamera")->SetPosition(cameraPosition);
	GetCamera("DirectionalLightCamera")->SetTarget(cameraTarget);
	GetCamera("DirectionalLightCamera")->SetUp(cameraUp);

	GetCamera("DirectionalLightCamera")->SetView();
	SetActiveCamera(GetCamera("DirectionalLightCamera"));

	// Get the matrices from the light point of view
	glm::mat4 modelLightMatrix = glm::mat4(1.0f);
	glm::mat4 viewLightMatrix = GetActiveCamera()->ViewMatrix();
	glm::mat4 projectionLightMatrix = GetActiveCamera()->ProjectionMatrix();

	// Compute shadow matrix
	shadowMatrix = biasMatrix * 
		projectionLightMatrix * 
		viewLightMatrix * 
		modelLightMatrix;

	// Bind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	
	// Attach the shadow map to the framebuffer
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D,
		shadowMapTexture,
		0);

	glDrawBuffer(GL_NONE);

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, shadowMapWidth, shadowMapHeight);
	
	simpleShader->Enable();
		glClear(GL_DEPTH_BUFFER_BIT);
		//DisplayObjects();
	simpleShader->Disable();
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_CULL_FACE);
	SetActiveCamera(GetCamera(0));
	glViewport(0, 0, 1024, 700);
}

void Scene::CreateDirectionalLight(glm::vec3 direction)
{
	// Add a new directional light to the light list

	// Initialize the properties of the directional light
	DirectionalLight dirLightProp;
	dirLightProp.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	dirLightProp.Direction = direction;
	dirLightProp.AmbientIntensity = 0.5f;
	dirLightProp.Direction = direction;
	dirLightProp.DiffuseIntensity = 0.8f;

	// Set the light parameters
	_Light->SetDirectionalLight(dirLightProp);

	// Set general light parameters
	_Light->SetEyeWorldPos(GetActiveCamera()->GetCameraPosition());
	_Light->SetSpecularIntensity(2.0f);
	_Light->SetSpecularPower(8.0f);

	//_Light->SetTextureUnit();
}

void Scene::CreatePointLights()
{
	// Data for point light 1
	pointLights.push_back(PointLight(
		glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f,
		0.7f,
		1.8f,
		0.9f,
		0.7f));

	// Data for point light 2
	pointLights.push_back(PointLight(
		glm::vec3(4.0f, 2.6f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		1.0f, 
		0.7f,
		1.8f,
		0.8f,
		0.4f));

	// Data for point light 3
	pointLights.push_back(PointLight(
		glm::vec3(2.0f, 6.0f, -1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		1.0f, 
		0.7f,
		1.8f,
		0.9f,
		0.3f));

	// Set the point lights
	_Light->SetPointLights(pointLights);
}

void Scene::UpdateLight(float dt)
{
	shadowShader->Enable();
		
	// Update the model matrix for the light object
	pointLights[0].TransformationMatrix = &GetObject("LightObject0")->GetModel();

	// Update the position of the light object 0
	glm::vec4 temp = (*pointLights[0].TransformationMatrix) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	pointLights[0].Position = glm::vec3(temp.x, temp.y, temp.z);

	// Set the point lights
	_Light->SetPointLights(pointLights);

	shadowShader->Disable();
}

GLuint Scene::createTexture(int w, int h, bool isDepth)
{
	// Clear the error
	glGetError();

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Create space for a texture
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		(!isDepth ? GL_RGBA8 : GL_DEPTH_COMPONENT),
		w,
		h,
		0,
		isDepth ? GL_DEPTH_COMPONENT : GL_RGBA,
		GL_FLOAT,
		NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// Error checking
	int i;
	i = glGetError();
	if (i != 0)
	{
		std::cout<<"Error while creating the texture."<<i<<std::endl;
	}

	// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

GLuint Scene::createFBO()
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);


	// Create a texture
	//renderTexture = createTexture(shadowMapWidth, shadowMapHeight, false);
	//depthTexture = createTexture(1024, 768, true);
	shadowMapTexture = createTexture(shadowMapWidth, shadowMapHeight, true);
	
	cubeTexture = createCubeTexture(CUBE_TEXTURE_SIZE, CUBE_TEXTURE_SIZE);
	int i = glGetError();
	if (i != 0)
	{
		std::cout<<"Error at quad geometry: "<<i<<std::endl;
	}
	// Attach a texture
	// When a texture i attached the rendering is done to the texture
	// Attachement - GL_COLOR_ATTACHEMENT0, 1
	//				- GL_DEPTH_ATTACHEMENT0
	//glFramebufferTexture2D(
	//	GL_FRAMEBUFFER,
	//	GL_COLOR_ATTACHMENT0,
	//	GL_TEXTURE_2D,
	//	renderTexture,
	//	0);

	//glFramebufferTexture2D(
	//	GL_FRAMEBUFFER,
	//	GL_DEPTH_ATTACHMENT,
	//	GL_TEXTURE_2D,
	//	depthTexture,
	//	0);// Detach the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	// Check for errors
	i = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (i != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout<<"Framebuffer is not OK: "<<i<<std::endl;
	}
	
	return FBO;
}

Mesh* Scene::createQuad()
{
	Mesh* newMesh = new Mesh();
	
	// 1 vertex
	newMesh->AddVertexPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	newMesh->AddVertexTexCoord(glm::vec2(0.0f, 0.0f));

	// 2 vertex
	newMesh->AddVertexPosition(glm::vec3(10.0f, 0.0f, 0.0f));
	newMesh->AddVertexTexCoord(glm::vec2(1.0f, 0.0f));

	// 3 vertex
	newMesh->AddVertexPosition(glm::vec3(10.0f, 10.0f, 0.0f));
	newMesh->AddVertexTexCoord(glm::vec2(1.0f, 1.0f));

	// 4 vertex
	newMesh->AddVertexPosition(glm::vec3(0.0f, 10.0f, 0.0f));
	newMesh->AddVertexTexCoord(glm::vec2(0.0f, 1.0f));
	
	/*newMesh->AddIndex(0);
	newMesh->AddIndex(1);
	newMesh->AddIndex(2);

	newMesh->AddIndex(0);
	newMesh->AddIndex(2);
	newMesh->AddIndex(3);*/
	newMesh->AddIndex(0);
	newMesh->AddIndex(1);
	newMesh->AddIndex(2);

	newMesh->AddIndex(2);
	newMesh->AddIndex(3);
	newMesh->AddIndex(0);

	return newMesh;
}

void Scene::LoadShaders()
{
	// Load the text shader 
	textShader = new Shader("..\\Shaders\\TextVertexShader.txt", "..\\Shaders\\TextFragmentShader.txt");
	textShader->LoadShaders();

	// Load the quad shader
	quadShader = new Shader("..\\Shaders\\quadVertexShader.txt", "..\\Shaders\\quadFragmentShader.txt");
	quadShader->LoadShaders();

	// Load the simple shader
	simpleShader = new Shader("..\\Shaders\\simpleVertexShader.txt", "..\\Shaders\\simpleFragmentShader.txt");
	simpleShader->LoadShaders();

	// Load the shadow shader
	shadowShader = new Shader("..\\Shaders\\cubeShadowVertexShader2.txt", "..\\Shaders\\cubeShadowFragmentShader2.txt");
	shadowShader->LoadShaders();
}

GLuint Scene::createCubeTexture(int w, int h)
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glGenTextures(1, &cubeTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);

   /* glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);*/

//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	int i = glGetError();
	if (i != 0)
	{
		std::cout<<"Error at cube tex: "<<i<<std::endl;
	}
	
    for (int loop = 0; loop < 6; ++loop)
    {
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + loop, 
			0, 
			GL_DEPTH_COMPONENT,
            w, 
			h, 
			0, 
			GL_DEPTH_COMPONENT, 
			GL_FLOAT, 
			NULL);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	return cubeTexture;
}


bool Scene::KeyPress(int key)
	{
		if (glfwGetKey( _window, key ) == GLFW_RELEASE)
		{
			if (glfwGetKey( _window, key) == GLFW_PRESS)
			{
				return true;
			}
		}
		return false;
	}

void Scene::DisplayFPS(float dt)
{
	static float frames = 0.0f;
	static float timeElapsed = 0.0f; 
	
	frames++;

	timeElapsed += dt;

	if ( timeElapsed >= 1.0 )
	{ 
		mFPS = frames;
		mMilisecondsPerFrame = 1000.0f / mFPS;
		
		timeElapsed = 0.0f;
		frames = 0.0f;
	}

	std::string val = static_cast<std::ostringstream*>( &(std::ostringstream() << mMilisecondsPerFrame) )->str();
	std::string temp = std::string("Miliseconds per frame:") + val;
	Text2D::PrintText2D(temp.c_str(), 20, 500, 10);

	val = static_cast<std::ostringstream*>( &(std::ostringstream() << double(mFPS)) )->str();
	temp = std::string("Frames per second:") + val;
	Text2D::PrintText2D(temp.c_str(), 20, 480, 10);
}