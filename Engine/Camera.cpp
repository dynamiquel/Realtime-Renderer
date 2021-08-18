#include "Camera.h"


#include "d3dclass.h"
#include "Logger.h"


Camera::Camera(Node* parent) : Actor(parent)
{
	skyDome = nullptr;
}

Camera::Camera(std::string name, Node* parent) : Actor(name, parent)
{
		
}

Camera::~Camera()
{
	// Release the sky dome object.
	if(skyDome)
	{
		delete skyDome;
		skyDome = nullptr;
	}
}

void Camera::SetLookAtPos(XMFLOAT3 lookAtPos)
{
	XMFLOAT3 rotation = GetLocalTransform().GetRotationEuler();
	
	// Don't update values if not needed.
	if (lookAtPos.x == rotation.x && lookAtPos.y == rotation.y && lookAtPos.z == rotation.z)
		return;

	lookAtPos.x = rotation.x - lookAtPos.x;
	lookAtPos.y = rotation.y - lookAtPos.y;
	lookAtPos.z = rotation.z - lookAtPos.z;

	float pitch = 0.f;
	if (lookAtPos.y != 0.f)
	{
		const float distance = sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
		pitch = atan(lookAtPos.y / distance);
	}

	float yaw = 0.f;
	if (lookAtPos.x != 0.f)
		yaw = atan(lookAtPos.x / lookAtPos.z);

	if (lookAtPos.z > 0.f)
		yaw += XM_PI;

	SetRotation(XMFLOAT3(XMConvertToDegrees(pitch), XMConvertToDegrees(yaw), 0.f));
}

void Camera::OnRender(D3DClass* d3d, ShaderManager* shaderManager, DirectX::XMMATRIX& worldMatrix,
	DirectX::XMMATRIX& viewMatrix, DirectX::XMMATRIX& projectionMatrix, const DirectX::BoundingFrustum& previousFrustum, Light* light, Camera* camera, bool depth, ID3D11ShaderResourceView* depthMapTexture)
{
	// Camera doesn't need depth. Ignore.
	if (depth)
		return;
	
	// Creates a new frustum to use for frustum culling.
	GetFrustum().CreateFromMatrix(frustum, projectionMatrix);
	
	// Sets the position of the frustum based off the camera's view matrix.
	// The view matrix needs to be inversed. Not sure why, but after 3 hours of work, it was the only way I was able to get it working.
	// If camera location was (20, -10, -50), the view matrix would keep going to (-20, 10, 50) and this would mess up the frustum.
	GetFrustum().Transform(frustum, XMMatrixInverse(nullptr, viewMatrix));

	// Debugging purposes - outputs the dimensions of the frustum.
	/*
	XMFLOAT3 Corners[8];
	GetFrustum().GetCorners(Corners);
	Logger::Log(LogSource::Camera, LogType::Log, "START");
	for (XMFLOAT3 corner : Corners)
		Logger::Log(LogSource::Camera, LogType::Log, StringHelper::ToString(corner));
	Logger::Log(LogSource::Camera, LogType::Log, "END");
	*/
	
	// Renders the Sky Dome around the camera.
	skyDome->Render(d3d, shaderManager, worldMatrix, viewMatrix, projectionMatrix, GetWorldTransform().GetLocation());
}

void Camera::OnUpdate(float deltaTime)
{

	XMFLOAT3 location = GetWorldTransform().GetLocation();
	const XMFLOAT3 rotation = GetWorldTransform().GetRotationEuler();
	
	// Setup the position of the camera in the world.
	const XMVECTOR positionVec = XMLoadFloat3(&location);
	
	// Setup the vector that points upwards.
	XMVECTOR up = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	// Setup where the camera is looking by default.
	XMVECTOR forward = XMVectorSet(0.f, 0.f, 1.f, 0.f);

	// Create the rotation matrix from the yaw, pitch, and roll values.
	const XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

	// Transform the forward and up vector by the rotation matrix so the view is correctly rotated at the origin.
	forward = XMVector3TransformCoord(forward, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	forward += positionVec;
	
	up = XMVector3TransformCoord(up, rotationMatrix);

	// Finally create the view matrix from the three updated vectors.
	viewMatrix = XMMatrixLookAtLH(positionVec, forward, up);

	//Logger::Log(LogSource::Camera, LogType::Log, StringHelper::ToString(m_viewMatrix));

	const XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, 0.f);
	
	// Stores the new directional vectors.
	XMStoreFloat3(&forwardVector, XMVector3TransformCoord(XMVectorSet(0.f, 0.f, 1.f, 0.f), vecRotationMatrix));
	XMStoreFloat3(&rightVector, XMVector3TransformCoord(XMVectorSet(1.f, 0.f, 0.f, 0.f), vecRotationMatrix));
	XMStoreFloat3(&upVector, XMVector3TransformCoord(XMVectorSet(0.f, 1.f, 0.f, 0.f), vecRotationMatrix));
}

void Camera::InitialiseSkyDome(D3DClass* d3d)
{
	// Creates the Sky Dome.
	skyDome = new SkyDome;
	skyDome->Initialize(d3d->GetDevice());
}
