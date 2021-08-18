#pragma once


#include "shaders/textureshaderclass.h"
#include "shaders/lightshaderclass.h"
#include "shaders/bumpmapshaderclass.h"
#include "shaders/depthshaderclass.h"
#include "shaders/shadowshaderclass.h"
#include "shaders/skydomeshaderclass.h"


class ShaderManager
{
public:
	ShaderManager();
	ShaderManager(const ShaderManager&);
	~ShaderManager();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();

	bool RenderTextureShader(ID3D11DeviceContext*, int, const XMMATRIX&, const XMMATRIX&, const XMMATRIX&, ID3D11ShaderResourceView*);

	bool RenderLightShader(ID3D11DeviceContext*, int, const XMMATRIX&, const XMMATRIX&, const XMMATRIX&, ID3D11ShaderResourceView*, Light*, const XMFLOAT3&);

	bool RenderBumpMapShader(ID3D11DeviceContext*, int, const XMMATRIX&, const XMMATRIX&, const XMMATRIX&, ID3D11ShaderResourceView*,
		ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4);

	bool RenderShadowShader(ID3D11DeviceContext*, int, const XMMATRIX&, const XMMATRIX&, const XMMATRIX&, ID3D11ShaderResourceView*, 
                             ID3D11ShaderResourceView*, Light* light);

	bool RenderDepthShader(ID3D11DeviceContext*, int, const XMMATRIX&, const XMMATRIX&, const XMMATRIX&);

	bool RenderSkyDomeShader(ID3D11DeviceContext*, int, const XMMATRIX&, const XMMATRIX&, const XMMATRIX&, const XMFLOAT4&, const XMFLOAT4&);

private:
	TextureShaderClass* textureShader;
	LightShaderClass* lightShader;
	BumpMapShaderClass* bumpMapShader;
	ShadowShaderClass* shadowShader;
	DepthShaderClass* depthShader;
	SkyDomeShaderClass* skyDomeShader;
};