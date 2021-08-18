#include "Engine.h"
#include "Logger.h"

// Program's entry point.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	bool result;
	
	// Creates the Engine object.
	Engine* engine = new Engine;
	if(!engine)
		return 0;

	// Initialize and runs the Engine.
	result = engine->Initialize(hInstance, "Engine", "MyEngine", 1920, 1080, false);
	if(result)
	{
		while (engine->ProcessMessages())
		{
			engine->Update();
			engine->RenderFrame();
		}
	}
	
	return 0;
}