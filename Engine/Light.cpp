////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Light.h"


Light::Light()
{
}


Light::Light(const Light& other)
{
}


Light::~Light()
{
}


void Light::SetAmbientColor(float red, float green, float blue, float alpha)
{
	ambientColor = XMFLOAT4(red, green, blue, alpha);
}


void Light::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	diffuseColor = XMFLOAT4(red, green, blue, alpha);
}


void Light::SetDirection(float x, float y, float z)
{
	direction = XMFLOAT3(x, y, z);
}

void Light::SetPosition(float x, float y, float z)
{
	position = XMFLOAT3(x, y, z);
}


void Light::SetSpecularColor(float red, float green, float blue, float alpha)
{
	specularColor = XMFLOAT4(red, green, blue, alpha);
}


void Light::SetSpecularPower(float power)
{
	specularPower = power;
}


XMFLOAT4 Light::GetAmbientColor()
{
	return ambientColor;
}


XMFLOAT4 Light::GetDiffuseColor()
{
	return diffuseColor;
}


XMFLOAT3 Light::GetDirection()
{
	return direction;
}


XMFLOAT4 Light::GetSpecularColor()
{
	return specularColor;
}


float Light::GetSpecularPower()
{
	return specularPower;
}

void Light::GenerateViewMatrix()
{
	XMFLOAT3 up = XMFLOAT3(0.f, 1.f, 0.f);
	
	viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&position), XMLoadFloat3(&direction), XMLoadFloat3(&up));
}

void Light::GenerateProjectiveMatrix(float screenDepth, float screenNear)
{
	float fov = XM_PI / 2.f;
	float screenAspect = 1.f;

	projectionMatrix = XMMatrixPerspectiveFovLH(fov, screenAspect, screenNear, screenDepth);
}

void Light::GenerateOrthographicMatrix(float width, float height, float screenDepth, float screenNear)
{
	orthoMatrix = XMMatrixOrthographicLH(width, height, screenNear, screenDepth);
}
