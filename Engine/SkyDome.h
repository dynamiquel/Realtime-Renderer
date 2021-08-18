#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>

#include "modelclass.h"
class ShaderManager;
class D3DClass;
using namespace std;


/**
 * \brief Renders a Sky Dome (similar to a skybox) around a Camera.
 */
class SkyDome
{

public:
	SkyDome();
	~SkyDome();

	void Render(D3DClass* d3d, ShaderManager* shaderManager, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix,
		XMMATRIX& projectionMatrix, const XMFLOAT3& cameraLocation);
	bool Initialize(ID3D11Device* device);

	XMFLOAT4 GetApexColor() const { return apexColor; }
	XMFLOAT4 GetCenterColor() const { return centerColor; }

private:
	ModelClass* model;
	XMFLOAT4 apexColor, centerColor;
};
