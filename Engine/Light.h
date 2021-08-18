////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LIGHT_H_
#define _LIGHT_H_


//////////////
// INCLUDES //
//////////////
#include <DirectXMath.h> 

using namespace DirectX;



////////////////////////////////////////////////////////////////////////////////
// Class name: LightClass
////////////////////////////////////////////////////////////////////////////////
class Light
{
public:
	Light();
	Light(const Light&);
	~Light();

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	void SetPosition(float, float, float);
	void SetSpecularColor(float, float, float, float);
	void SetSpecularPower(float);

	XMFLOAT4  GetAmbientColor();
	XMFLOAT4  GetDiffuseColor();
	XMFLOAT3  GetDirection();
	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT4  GetSpecularColor();
	float GetSpecularPower();

	void GenerateViewMatrix();
	void GenerateProjectiveMatrix(float screenDepth, float screenNear);
	void GenerateOrthographicMatrix(float width, float height, float screenDepth, float screenNear);

	XMMATRIX& GetViewMatrix() { return viewMatrix; }
	XMMATRIX& GetProjectionMatrix() { return projectionMatrix; }
	XMMATRIX& GetOrthographicMatrix() { return orthoMatrix; }

private:
	XMFLOAT4  ambientColor;
	XMFLOAT4  diffuseColor;
	XMFLOAT3  direction;
	XMFLOAT3  position;
	XMFLOAT4  specularColor;
	float specularPower;

	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
	XMMATRIX orthoMatrix;
};

#endif