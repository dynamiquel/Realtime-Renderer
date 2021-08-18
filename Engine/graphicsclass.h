////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Actor.h"
#include "d3dclass.h"


class SkyDome;
class RenderTexture;
class ShaderManager;

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = .1f;
const int SHADOWMAP_WIDTH = 1920;
const int SHADOWMAP_HEIGHT = 1080;
const float SHADOWMAP_DEPTH = 50.0f;
const float SHADOWMAP_NEAR = 1.0f;

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	bool InitialiseModel(Actor* actor, char* modelFilename, std::wstring textureFilename) const;
	void Shutdown();
	bool Frame(Scene* sceneNode, Camera* camera);

	D3DClass* GetD3D() const { return m_D3D; }

private:
	bool Render();
	void RenderScene();
	bool RenderSceneToTexture() const;

private:
	D3DClass* m_D3D;
	ShaderManager* m_ShaderManager;
	Camera* m_Camera;
	Light* m_Light;
	Scene* m_sceneNode;
	RenderTexture* m_RenderTexture;
};

#endif