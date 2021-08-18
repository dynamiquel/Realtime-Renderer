////////////////////////////////////////////////////////////////////////////////
// Filename: skydomeclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "SkyDome.h"


#include "Camera.h"
#include "d3dclass.h"
#include "Logger.h"
#include "ShaderManager.h"

SkyDome::SkyDome()
{
	// Set the color at the top of the sky dome.
	apexColor = XMFLOAT4(67.f/256.f, 118.f/256.f, 169.f/256.f, 1.0f);
	//apexColor = XMFLOAT4(118.f/256.f, 55.f/256.f,  62.f/256.f, 1.0f);
	
	// Set the color at the center of the sky dome.
	centerColor =  XMFLOAT4(.7f, .91f, .95f, 1.0f);
	//centerColor = XMFLOAT4(62.f/256.f, 33.f/256.f, 84.f/256.f, 1.0f);

	model = new ModelClass;
}

SkyDome::~SkyDome()
{
	if (model)
	{
		model->Shutdown();
		delete model;
		model = nullptr;
	}
}

void SkyDome::Render(D3DClass* d3d, ShaderManager* shaderManager, XMMATRIX& worldMatrix,
	XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, const XMFLOAT3& cameraLocation)
{
	// Since the camera will be inside of the Sky Dome, the renderer needs to stop culling the back facing triangles.
	// Otherwise, the Sky Dome will not be visible.
	d3d->SetCullMode(D3D11_CULL_FRONT);

	// Scales the Sky Dome.
	const XMMATRIX scaleMatrix = XMMatrixScaling(500.f, 500.f, 500.f);
	worldMatrix = scaleMatrix;
	
	// Sky Dome positions itself from the Camera's origin.
	const XMMATRIX locationMatrix = XMMatrixTranslation(cameraLocation.x, cameraLocation.y, cameraLocation.z);
	worldMatrix *= locationMatrix;

	// Renders the model (the sphere) of the Sky Dome.
	model->Render(d3d->GetDeviceContext());

	// Renders the shader of the Sky Dome.
	shaderManager->RenderSkyDomeShader(d3d->GetDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, GetApexColor(), GetCenterColor());

	// Re-enables the culling of back-facing triangles (default behaviour).
	d3d->SetCullMode(D3D11_CULL_BACK);
}


bool SkyDome::Initialize(ID3D11Device* device)
{
	// Sets up the spherical model to use as the Sky Dome.
	if (!model->Initialize(device, "../Engine/data/SkyDome.txt"))
	{
		Logger::Log("Could not initialise Sky Dome model object");
		return false;
	}
	
	return true;
}