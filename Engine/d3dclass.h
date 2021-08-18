////////////////////////////////////////////////////////////////////////////////
// Filename: d3dclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

//////////////
// INCLUDES //
//////////////
#include <dxgi.h>
#include <d3d11_1.h>
#include <DirectXMath.h>

using namespace DirectX;


////////////////////////////////////////////////////////////////////////////////
// Class name: D3DClass
////////////////////////////////////////////////////////////////////////////////
class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, 
                          float screenDepth, float screenNear);
	void Shutdown();
	
	void BeginScene(float red, float green, float blue, float alpha);
	void EndScene();

	ID3D11Device* GetDevice() const { return m_device; }
	ID3D11DeviceContext* GetDeviceContext() const { return m_deviceContext; }
	void GetVideoCardInfo(char* cardName, int& memory);


	XMMATRIX& GetProjectionMatrix() { return m_projectionMatrix; }
	XMMATRIX& GetWorldMatrix() { return m_worldMatrix; }
	XMMATRIX& GetOrthoMatrix() { return m_orthoMatrix; }

	void SetBackBufferRenderTarget();
	void ResetViewport();
	void SetCullMode(D3D11_CULL_MODE cullMode);

private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	
	ID3D11RasterizerState* m_rasterStateBackCulling;
	ID3D11RasterizerState* m_rasterStateFrontCulling;
	ID3D11RasterizerState* m_rasterStateNoCulling;

	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;
	D3D11_VIEWPORT m_viewport;
};

#endif