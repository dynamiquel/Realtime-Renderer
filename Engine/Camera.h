#pragma once


#include <DirectXMath.h> 
#include "Actor.h"
#include "directxcollision.h"
#include "SkyDome.h"

class Camera : public Actor
{
public:
	Camera(Node* parent);
	Camera(std::string name, Node* parent);
	Camera(const Camera&);
	~Camera();

	XMMATRIX& GetViewMatrix() { return viewMatrix; }
	const BoundingFrustum& GetFrustum() const { return frustum; }
	
	void SetLookAtPos(XMFLOAT3 lookAtPos);

	void OnRender(D3DClass* d3d, ShaderManager* shaderManager, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix, const BoundingFrustum& frustum, Light* light, Camera* camera, bool depth, ID3D11ShaderResourceView* depthMapTexture) override;
	void OnUpdate(float deltaTime) override;
	
	void InitialiseSkyDome(D3DClass* d3d);

private:
	XMMATRIX viewMatrix;

	// Frustum used to detect which Actors are visible by the camera.
	BoundingFrustum frustum;

	// Sky Dome used to project a "skybox" around the camera.
	SkyDome* skyDome;
};
