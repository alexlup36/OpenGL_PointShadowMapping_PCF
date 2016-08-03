#include "Util.h"

#include "Engine.h"

int main()
{
	// ------------------------------------------------------------------------

	// Create the engine
	Engine* engine = new Engine();

	// Initialize the engine's window
	if (!engine->InitializeWindow())
	{
		fprintf(stderr, "Failed to initialize the window\n");
		return 0;
	}

	// Set the properties of the engine's window
	engine->SetWindowProperties("TechDemo");

	// Set up the scene
	engine->SetUpScene();

	engine->Run();

	delete engine;

	return 0;
}