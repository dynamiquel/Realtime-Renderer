////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"

#include <queue>

#include "Actor.h"
#include "Light.h"
#include "Logger.h"
#include "Camera.h"
#include "RenderTexture.h"
#include "Scene.h"
#include "ShaderManager.h"
#include "SkyDome.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = nullptr;
	m_ShaderManager = nullptr;
	m_Light = nullptr;
	m_Camera = nullptr;
	m_sceneNode = nullptr;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
		return false;

	// Initialize the Direct3D object.
	if (!m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the shader manager object.
	m_ShaderManager = new ShaderManager;
	if(!m_ShaderManager)
		return false;

	// Initialize the shader manager object.
	if (!m_ShaderManager->Initialize(m_D3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the Shader Manager object.", L"Error", MB_OK);
		return false;
	}

	// Create the render to texture object.
	m_RenderTexture = new RenderTexture;
	if(!m_RenderTexture)
		return false;

	// Initialize the render to texture object.
	if (!m_RenderTexture->Initialize(m_D3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SHADOWMAP_DEPTH, SHADOWMAP_NEAR))
	{
		MessageBox(hwnd, L"Could not initialize the Render Texture object.", L"Error", MB_OK);
		return false;
	}

		
	// Create the light object.
	m_Light = new Light;
	if(!m_Light)
		return false;

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(XMConvertToRadians(50.f), XMConvertToRadians(-20.f), XMConvertToRadians(0.f));
	m_Light->SetPosition(-20.f, 100.f, -20.f);
	//m_Light->SetPosition(0.f, 100.f, 0.f);
	//m_Light->SetDirection(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f));
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(64.0f);
	m_Light->GenerateProjectiveMatrix(SCREEN_DEPTH, SCREEN_NEAR);
	m_Light->GenerateOrthographicMatrix(15.f, 15.f, SHADOWMAP_DEPTH, SHADOWMAP_NEAR);

	return true;
}

bool GraphicsClass::InitialiseModel(Actor* actor, char* modelFilename, std::wstring textureFilename) const
{
	if (!actor->GetModel()->Initialize(m_D3D->GetDevice(), modelFilename, textureFilename))
	{
		Logger::Log("Could not initialise model object");
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the objects.
	queue<Node*> queue;
	queue.push(m_sceneNode);

	while (queue.size() > 0)
	{
		Node* node = queue.front();
		queue.pop();	
		
		for (Node* childNode : node->GetChildren())
			queue.push(childNode);

		if (node)
			delete node;
	}

	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = nullptr;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = nullptr;
	}

	// Release the shader manager object.
	if(m_ShaderManager)
	{
		m_ShaderManager->Shutdown();
		delete m_ShaderManager;
		m_ShaderManager = nullptr;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = nullptr;
	}

	// Release the render texture object.
	if(m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = nullptr;
	}
}


bool GraphicsClass::Frame(Scene* sceneNode, Camera* camera)
{
	m_sceneNode = sceneNode;
	m_Camera = camera;
	
	// Render the graphics scene.
	if (Render())
		return false;

	return true;
}

void GraphicsClass::RenderScene()
{	
	XMMATRIX worldMatrix = m_D3D->GetWorldMatrix();
	XMMATRIX projectionMatrix = m_D3D->GetProjectionMatrix();
	XMMATRIX viewMatrix = m_Camera->GetViewMatrix();

	// Creates a queue and pushes the Scene (root) node to it.
	queue<Node*> queue;
	queue.push(m_sceneNode);

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

		node->OnRender(m_D3D, m_ShaderManager, worldMatrix, viewMatrix, projectionMatrix, m_Camera->GetFrustum(),
			m_Light, m_Camera, false, m_RenderTexture->GetShaderResourceView());

		// Adds the children of the node to the end of the queue.
		for (Node* childNode : node->GetChildren())
			queue.push(childNode);
	}
}

bool GraphicsClass::RenderSceneToTexture() const
{	
	XMMATRIX worldMatrix = m_D3D->GetWorldMatrix();
	XMMATRIX projectionMatrix = m_D3D->GetProjectionMatrix();
	XMMATRIX viewMatrix = m_Camera->GetViewMatrix();
	
	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(m_D3D->GetDeviceContext());

	// Clear the render to texture.
	m_RenderTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	m_Light->GenerateViewMatrix();

	queue<Node*> queue;
	queue.push(m_sceneNode);

	while (queue.size() > 0)
	{
		Node* node = queue.front();
		queue.pop();

		if (!node->enabled)
			continue;

		node->OnRender(m_D3D, m_ShaderManager, worldMatrix, viewMatrix, projectionMatrix, m_Camera->GetFrustum(), m_Light, m_Camera, true, m_RenderTexture->GetShaderResourceView());
		
		for (Node* childNode : node->GetChildren())
			queue.push(childNode);
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_D3D->ResetViewport();

	return true;
}

bool GraphicsClass::Render()
{
	if (!RenderSceneToTexture())
		Logger::Log(LogSource::Rendering, LogType::Error, "Cannot render scene to texture");
	
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Light->GenerateViewMatrix();

	// Renders everything in the Scene hierarchy.
	RenderScene();
	
	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}
