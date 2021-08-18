#include "Engine.h"

#include <queue>


#include "AssetManager.h"
#include "TimedActor.h"
#include "Logger.h"
#include "MathHelper.h"
#include "Camera.h"
#include "Scene.h"
#include "StringHelper.h"

void Engine::Exit()
{
	if (instance)
		instance->Shutdown();
}

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height, bool fullscreen)
{
	// Initialises the Engine singleton.
	if (instance != nullptr)
	{
		Logger::Log("An Engine instance already exists.");
		return false;
	}
	instance = this;

	Logger::Initialise();

	// Starts the delta timer.
	timer.Start();

	if (!Window::Initialize(hInstance, window_title, window_class, width, height, fullscreen))
	{
		Logger::Log(LogSource::Application, LogType::Error, "Window couldn't be initialised.");
		return false;
	}
	
	Logger::Log(LogSource::Application, LogType::Log, "Window initialised.");

	gfx = new GraphicsClass;
	if (!gfx->Initialize(width, height, GetHWND()))
	{
		Logger::Log(LogSource::Rendering, LogType::Error, "GraphicsClass couldn't be initialised.");
		return false;
	}

	Logger::Log(LogSource::Rendering, LogType::Log, "GraphicsClass initialised.");

	PreloadAssets();
	Logger::Log(LogSource::Assets, LogType::Log, "Assets preloaded.");

	if (!InitialiseScene())
	{
		Logger::Log(LogSource::Scene, LogType::Error, "Scene couldn't be initialised.");
		return false;
	}

	Logger::Log(LogSource::Scene, LogType::Log, "Scene initialised.");

	return true;
}

bool Engine::ProcessMessages()
{
	return Window::ProcessMessages();
}

void Engine::Update()
{
	// Updates the Input.
	GetInput()->Frame();

	// Gets the delta time and starts recording the delta time for the next frame.
	const float deltaTime = timer.GetMillisecondsElapsed() * 0.001;
	timer.Restart();

	// Update the Nodes in the Scene hierarchy.
	UpdateScene(deltaTime);

	float cameraSpeed = 10.f;
	if (GetInput()->GetKeyDown(DIK_LSHIFT))
		cameraSpeed *= 4.f;

	float rotateSpeed = .0005f;

	// --- INPUT STUFF ---
	//Logger::Log(LogSource::Input, LogType::Log, std::to_string(GetInput()->GetMouseX()) + ", " + std::to_string(GetInput()->GetMouseY()));
	//Logger::Log(LogSource::Camera, LogType::Log, StringHelper::ToString(camera->GetUpVector()));
	if (GetInput()->GetKeyDown(DIK_W))
		camera->AdjustLocation(MathHelper::MultiplyVector(camera->GetForwardVector(), cameraSpeed * deltaTime));
	if (GetInput()->GetKeyDown(DIK_S))
		camera->AdjustLocation(MathHelper::MultiplyVector(camera->GetForwardVector(), -cameraSpeed * deltaTime));
	if (GetInput()->GetKeyDown(DIK_A))
		camera->AdjustLocation(MathHelper::MultiplyVector(camera->GetRightVector(), -cameraSpeed * deltaTime));
	if (GetInput()->GetKeyDown(DIK_D))
		camera->AdjustLocation(MathHelper::MultiplyVector(camera->GetRightVector(), cameraSpeed * deltaTime));
	if (GetInput()->GetKeyDown(DIK_Q))
		camera->AdjustLocation(MathHelper::MultiplyVector(camera->GetUpVector(), -cameraSpeed * deltaTime));
	if (GetInput()->GetKeyDown(DIK_E))
		camera->AdjustLocation(MathHelper::MultiplyVector(camera->GetUpVector(), cameraSpeed * deltaTime));

	// Adjusts camera rotation based off key input.
	/*if (GetInput()->GetKeyDown(DIK_UPARROW))
		camera->AdjustRotation(XMFLOAT3(-rotateSpeed * dt , 0.f, 0.f));
	if (GetInput()->GetKeyDown(DIK_DOWNARROW))
		camera->AdjustRotation(XMFLOAT3(rotateSpeed * dt, 0.f, 0.f));
	if (GetInput()->GetKeyDown(DIK_LEFTARROW))
		camera->AdjustRotation(XMFLOAT3(0.f, -rotateSpeed * dt, 0.f));
	if (GetInput()->GetKeyDown(DIK_RIGHTARROW))
		camera->AdjustRotation(XMFLOAT3(0.f, rotateSpeed * dt, 0.f));
	*/

	// Adjusts camera rotation based off mouse movement.
	int deltaMouseX, deltaMouseY;
	GetInput()->GetDeltaMouse(deltaMouseX, deltaMouseY);
	camera->AdjustRotation(XMFLOAT3(rotateSpeed * (float)deltaMouseY, rotateSpeed * (float)deltaMouseX, 0.f));
	
	if (GetInput()->GetKeyDown(DIK_ESCAPE))
		Exit();

	// Spawns a sphere in front of the camera.
	if (GetInput()->GetMouseButtonDown(0))
	{
		auto* actor = new Actor("Sphere", sceneNode);
		XMFLOAT3 location = MathHelper::AddVector(camera->GetLocation(), MathHelper::MultiplyVector(camera->GetForwardVector(), 10.f));
		actor->SetLocation(location);
		actor->SetRotation(camera->GetRotation());
		gfx->InitialiseModel(actor, "../Engine/data/cube.txt", L"../Engine/data/metal.dds");
	}
	
	// Spawns a timed sphere in front of the camera.
	if (GetInput()->GetMouseButtonDown(1))
	{
		auto* actor = new TimedActor("TimedSphere", sceneNode);
		XMFLOAT3 location = MathHelper::AddVector(camera->GetLocation(), MathHelper::MultiplyVector(camera->GetForwardVector(), 10.f));
		actor->SetLocation(location);
		actor->SetRotation(camera->GetRotation());
		gfx->InitialiseModel(actor, "../Engine/data/sphere.txt", L"../Engine/data/stone.dds");
	}
	// --- END OF INPUT STUFF ---

	
	//camera->SetLookAtPos(XMFLOAT3(0.f, 0.f, 0.f));
}

void Engine::RenderFrame()
{
	this->gfx->Frame(sceneNode, camera);
}

void Engine::Shutdown()
{
	Logger::Log(LogSource::Application, LogType::Log, "Application shutting down...");
	
	if (gfx)
	{
		Logger::Log(LogSource::Rendering, LogType::Log, "GraphicsClass shutting down...");

		gfx->Shutdown();
		delete gfx;
		gfx = nullptr;

		Logger::Log(LogSource::Rendering, LogType::Log, "GraphicsClass shut down.");
	}

	Logger::Log(LogSource::Application, LogType::Log, "Window shutting down...");
	Window::Shutdown();
	Logger::Log(LogSource::Application, LogType::Log, "Window shut down.");
}

bool Engine::InitialiseScene()
{
	sceneNode = new Scene();

	camera = new Camera("Camera", sceneNode);
	camera->InitialiseSkyDome(gfx->GetD3D());

	// Set the initial position of the camera.
	// Birds-eye
	//m_Camera->SetPosition(-30.f, 40.f, -30.f);
	//m_Camera->SetRotation(50.f, 45.f, 0.f);

	// Front-view
	camera->SetLocation(XMFLOAT3(0.f, 0.f, -50.f));

	// Side-view
	//m_Camera->SetPosition(-50.f, 0.f, 0.f);
	//m_Camera->SetRotation(0.f, 90.f, 0.f);

	// Top view
	//m_Camera->SetPosition(0.f, 50.f, 0.f);
	//m_Camera->SetRotation(90.f, 0.f, 0.f);

	// Inside view
	//m_Camera->SetPosition(-2.f, 1.f, 3.f);
	//m_Camera->SetRotation(0.f, 80.f, 0.f);

	// Debugging actors.
	/*Actor* topLeft = new Actor("TopLeft", sceneNode);
	topLeft->GetLocalTransform().SetLocation(-20.f, 20.f, 0.f);
	gfx->InitialiseModel(topLeft, "../Engine/data/sphere.txt", L"../Engine/data/metal.dds");

	Actor* topRight = new Actor("TopRight", sceneNode);
	topRight->GetLocalTransform().SetLocation(20.f, 20.f, 0.f);
	gfx->InitialiseModel(topRight, "../Engine/data/sphere.txt", L"../Engine/data/metal.dds");

	Actor* bottomLeft = new Actor("BottomLeft", sceneNode);
	bottomLeft->GetLocalTransform().SetLocation(20.f, -20.f, 0.f);
	gfx->InitialiseModel(bottomLeft, "../Engine/data/sphere.txt", L"../Engine/data/stone.dds");

	Actor* bottomRight = new Actor("BottomRight", sceneNode);
	bottomRight->GetLocalTransform().SetLocation(-20.f, -20.f, 0.f);
	gfx->InitialiseModel(bottomRight, "../Engine/data/sphere.txt", L"../Engine/data/stone.dds");

	Actor* closest = new Actor("Closest", sceneNode);
	closest->GetLocalTransform().SetLocation(0.f, 0.f, -20.f);
	gfx->InitialiseModel(closest, "../Engine/data/sphere.txt", L"../Engine/data/stone.dds");

	Actor* farthest = new Actor("Farthest", sceneNode);
	farthest->GetLocalTransform().SetLocation(0.f, 0.f, 20.f);
	gfx->InitialiseModel(farthest, "../Engine/data/sphere.txt", L"../Engine/data/metal.dds");
	*/

	// Test area

	Actor* temp;
	temp = new Actor("Landscape", sceneNode);
	/*temp->GetLocalTransform().SetLocation(50.f, 80.f, -100.f); sun location
	temp->GetLocalTransform().SetRotation(120.f, -20.f, 0.f);*/
	gfx->InitialiseModel(temp, "../Engine/data/Landscape.txt", L"../Engine/data/dirt.dds");

	temp = new Actor("Cube1", sceneNode);
	temp->GetLocalTransform().SetLocation(8.5f, 2.2f, 9.f);
	temp->GetLocalTransform().SetScale(2.f, 2.f, 2.f);
	gfx->InitialiseModel(temp, "../Engine/data/Cube.txt", L"../Engine/data/metal.dds");

	temp = new Actor("Cube2", sceneNode);
	temp->GetLocalTransform().SetLocation(7.07f, 2.47f, 9.f);
	temp->GetLocalTransform().SetScale(3.f, 3.f, 3.f);
	gfx->InitialiseModel(temp, "../Engine/data/Cube.txt", L"../Engine/data/metal.dds");

	temp = new Actor("Cube3", sceneNode);
	temp->GetLocalTransform().SetLocation(4.9f, 2.75f, 9.f);
	temp->GetLocalTransform().SetScale(4.f, 4.f, 4.f);
	gfx->InitialiseModel(temp, "../Engine/data/Cube.txt", L"../Engine/data/metal.dds");

	temp = new Actor("Cube4", sceneNode);
	temp->GetLocalTransform().SetLocation(2.35f, 3.f, 9.f);
	temp->GetLocalTransform().SetScale(8.f, 8.f, 8.f);
	gfx->InitialiseModel(temp, "../Engine/data/Cube.txt", L"../Engine/data/metal.dds");

	temp = new Actor("Cube4", sceneNode);
	temp->GetLocalTransform().SetLocation(-2.05f, 3.9f, 9.f);
	temp->GetLocalTransform().SetScale(10.f, 10.f, 10.f);
	gfx->InitialiseModel(temp, "../Engine/data/Cube.txt", L"../Engine/data/metal.dds");


	temp = new Actor("CentreRamp", sceneNode);
	temp->GetLocalTransform().SetLocation(3.7f, 2.56f, 0.f);
	temp->GetLocalTransform().SetScale(1.f, 1.f, 1.f);
	gfx->InitialiseModel(temp, "../Engine/data/Ramp3.txt", L"../Engine/data/metal.dds");

	temp = new Actor("Ramp", sceneNode);
	temp->GetLocalTransform().SetLocation(-11.13f, 2.56f, 0.f);
	temp->GetLocalTransform().SetScale(1.f, 1.f, 1.f);
	gfx->InitialiseModel(temp, "../Engine/data/Ramp1.txt", L"../Engine/data/metal.dds");

	temp = new Actor("BeveledRamp", sceneNode);
	temp->GetLocalTransform().SetLocation(-11.25f, 2.59f, -3.985f);
	temp->GetLocalTransform().SetScale(1.f, 1.f, 1.f);
	gfx->InitialiseModel(temp, "../Engine/data/Ramp2.txt", L"../Engine/data/metal.dds");


	temp = new Actor("BeveledCube", sceneNode);
	temp->GetLocalTransform().SetLocation(7.38f, 2.73f, -9.5f);
	temp->GetLocalTransform().SetScale(1.f, 1.f, 1.f);
	gfx->InitialiseModel(temp, "../Engine/data/BeveledCube.txt", L"../Engine/data/metal.dds");

	temp = new Actor("Pyramid", sceneNode);
	temp->GetLocalTransform().SetLocation(4.875f, 2.76f, -9.39f);
	temp->GetLocalTransform().SetScale(1.f, 1.f, 1.f);
	gfx->InitialiseModel(temp, "../Engine/data/Pyramid.txt", L"../Engine/data/metal.dds");

	temp = new Actor("BeveledPyramid", sceneNode);
	temp->GetLocalTransform().SetLocation(2.235f, 2.767f, -9.39f);
	temp->GetLocalTransform().SetScale(1.f, 1.f, 1.f);
	gfx->InitialiseModel(temp, "../Engine/data/BeveledPyramid.txt", L"../Engine/data/metal.dds");

	temp = new Actor("Roof", sceneNode);
	temp->GetLocalTransform().SetLocation(19.0478f, 7.45004f, 0.164316f);
	temp->GetLocalTransform().SetRotation(90.f, 0.f, 0.f);
	temp->GetLocalTransform().SetScale(1.f, 1.f, 1.f);
	gfx->InitialiseModel(temp, "../Engine/data/Wall.txt", L"../Engine/data/metal.dds");

	temp = new Actor("XWall", sceneNode);
	temp->GetLocalTransform().SetLocation(22.055f, 4.463f, 0.155f);
	temp->GetLocalTransform().SetRotation(0.f, 90.f, 0.f);
	temp->GetLocalTransform().SetScale(1.f, 1.f, 1.f);
	gfx->InitialiseModel(temp, "../Engine/data/Wall.txt", L"../Engine/data/metal.dds");

	temp = new Actor("ZWall", sceneNode);
	temp->GetLocalTransform().SetLocation(19.0478f, 4.463f, 3.14112f);
	temp->GetLocalTransform().SetRotation(0.f, 0.f, 0.f);
	temp->GetLocalTransform().SetScale(1.f, 1.f, 1.f);
	gfx->InitialiseModel(temp, "../Engine/data/Wall.txt", L"../Engine/data/metal.dds");

	temp = new Actor("-ZWall", sceneNode);
	temp->GetLocalTransform().SetLocation(19.0478f, 4.463f, -2.84758f);
	temp->GetLocalTransform().SetRotation(0.f, 0.f, 0.f);
	temp->GetLocalTransform().SetScale(1.f, 1.f, 1.f);
	gfx->InitialiseModel(temp, "../Engine/data/Wall.txt", L"../Engine/data/metal.dds");
	
	temp = new Actor("Bow", sceneNode);
	temp->GetLocalTransform().SetLocation(-200.f, 60.f, 400.f);
	temp->GetLocalTransform().SetRotation(0.f, 0.f, -60.f);
	temp->GetLocalTransform().SetScale(1.f, 1.f, 1.f);
	gfx->InitialiseModel(temp, "../Engine/data/bow.txt", L"../Engine/data/metal.dds");

	
	return true;
}

void Engine::PreloadAssets()
{
	// Kinda useless for this demo but it allows assets to be loaded when the engine starts instead of on demand.
	Logger::Log(LogSource::Assets, LogType::Log, "Preloading Assets...");

	vector<wstring> assetsToPreload;
	assetsToPreload.push_back(L"../Engine/data/metal.dds");

	for (auto asset : assetsToPreload)
	{
		AssetManager::GetAsset(gfx->GetD3D()->GetDevice(), asset);
	}	
}

void Engine::UpdateScene(const float deltaTime)
{
	// Creates a queue and pushes the Scene (root) node to it.
	queue<Node*> queue;
	queue.push(sceneNode);

	//Logger::Log(LogSource::Camera, LogType::Log, StringHelper::ToString(m_Camera->GetFrustum().Orientation));

	// Renders everything in the Scene hierarchy using a breadth-first approach.
	while (queue.size() > 0)
	{
		// Gets the next node in the queue.
		Node* node = queue.front();
		// Removes this node from the queue since it doesn't need to be rendered again.
		queue.pop();

		// If the node is disabled, skip this node and all of its children.
		if (!node->enabled)
			continue;

		node->OnUpdate(deltaTime);

		// Adds the children of the node to the end of the queue.
		for (Node* childNode : node->GetChildren())
			queue.push(childNode);
	}
}
