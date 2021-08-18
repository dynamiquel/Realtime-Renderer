#include "ShaderManager.h"

#include "Logger.h"


ShaderManager::ShaderManager()
{
	textureShader = nullptr;
	lightShader = nullptr;
	bumpMapShader = nullptr;
}


ShaderManager::ShaderManager(const ShaderManager& other)
{
}


ShaderManager::~ShaderManager()
{
}


bool ShaderManager::Initialize(ID3D11Device* device, HWND hwnd)
{
	// Create the texture shader object.
	textureShader = new TextureShaderClass;
	if(!textureShader)
	{
		Logger::Log(LogSource::Shaders, LogType::Error, "Could not create the Texture Shader");
		return false;
	}

	// Initialize the texture shader object.
	if (!textureShader->Initialize(device, hwnd))
	{
		Logger::Log(LogSource::Shaders, LogType::Error, "Could not initialise the Texture Shader");
		return false;
	}

	// Create the light shader object.
	lightShader = new LightShaderClass;
	if(!lightShader)
	{
		Logger::Log(LogSource::Shaders, LogType::Error, "Could not create the Light Shader");
		return false;
	}

	// Initialize the light shader object.
	if (!lightShader->Initialize(device, hwnd))
	{
		Logger::Log(LogSource::Shaders, LogType::Error, "Could not initialise the Light Shader");
		return false;
	}

	// Create the bump map shader object.
	bumpMapShader = new BumpMapShaderClass;
	if(!bumpMapShader)
	{
		Logger::Log(LogSource::Shaders, LogType::Error, "Could not create the Bump Map Shader");
		return false;
	}

	// Initialize the bump map shader object.
	if (!bumpMapShader->Initialize(device, hwnd))
	{
		Logger::Log(LogSource::Shaders, LogType::Error, "Could not initialise the Bump Map Shader");
		return false;
	}

	// Create the shadow shader object.
	shadowShader = new ShadowShaderClass;
	if(!shadowShader)
	{
		Logger::Log(LogSource::Shaders, LogType::Error, "Could not create the Shadow Shader");
		return false;
	}

	// Initialize the shadow shader object.
	if (!shadowShader->Initialize(device, hwnd))
	{
		Logger::Log(LogSource::Shaders, LogType::Error, "Could not initialise the Shadow Shader");
		return false;
	}

	// Create the depth shader object.
	depthShader = new DepthShaderClass;
	if(!depthShader)
	{
		Logger::Log(LogSource::Shaders, LogType::Error, "Could not create the Depth Shader");
		return false;
	}

	// Initialize the depth shader object.
	if (!depthShader->Initialize(device, hwnd))
	{
		Logger::Log(LogSource::Shaders, LogType::Error, "Could not initialise the Depth Shader");
		return false;
	}


	// Create the sky dome shader object.
	skyDomeShader = new SkyDomeShaderClass;
	if(!skyDomeShader)
	{
		Logger::Log(LogSource::Shaders, LogType::Error, "Could not create the Sky Dome Shader");
		return false;
	}

	// Initialize the depth shader object.
	if (!skyDomeShader->Initialize(device, hwnd))
	{
		Logger::Log(LogSource::Shaders, LogType::Error, "Could not initialise the Sky Dome Shader");
		return false;
	}

	return true;
}


void ShaderManager::Shutdown()
{
	// Release the bump map shader object.
	if(bumpMapShader)
	{
		bumpMapShader->Shutdown();
		delete bumpMapShader;
		bumpMapShader = nullptr;
	}

	// Release the light shader object.
	if(lightShader)
	{
		lightShader->Shutdown();
		delete lightShader;
		lightShader = nullptr;
	}

	// Release the texture shader object.
	if(textureShader)
	{
		textureShader->Shutdown();
		delete textureShader;
		textureShader = nullptr;
	}

	// Release the shadow shader object.
	if(shadowShader)
	{
		shadowShader->Shutdown();
		delete shadowShader;
		shadowShader = nullptr;
	}

	// Release the depth shader object.
	if(depthShader)
	{
		depthShader->Shutdown();
		delete depthShader;
		depthShader = nullptr;
	}

	// Release the sky dome shader object.
	if(skyDomeShader)
	{
		skyDomeShader->Shutdown();
		delete skyDomeShader;
		skyDomeShader = nullptr;
	}

	return;
}


bool ShaderManager::RenderTextureShader(ID3D11DeviceContext* device, int indexCount, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix,
											 ID3D11ShaderResourceView* texture)
{
	// Render the model using the texture shader.
	return textureShader->Render(device, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture);
}


bool ShaderManager::RenderLightShader(ID3D11DeviceContext* deviceContext, int indexCount, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix,
	ID3D11ShaderResourceView* texture, Light* light, const XMFLOAT3& cameraPosition)
{
	// Render the model using the light shader.
	return lightShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, light, cameraPosition);
}


bool ShaderManager::RenderBumpMapShader(ID3D11DeviceContext* deviceContext, int indexCount, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix,
	ID3D11ShaderResourceView* colorTexture, ID3D11ShaderResourceView* normalTexture, XMFLOAT3 lightDirection, XMFLOAT4 diffuse)
{
	// Render the model using the bump map shader.
	return bumpMapShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, colorTexture, normalTexture, lightDirection, diffuse);
}

bool ShaderManager::RenderShadowShader(ID3D11DeviceContext* deviceContext, int indexCount, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix,
	ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* depthMapTexture, Light* light)
{
	return shadowShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, depthMapTexture, light);
}

bool ShaderManager::RenderDepthShader(ID3D11DeviceContext* deviceContext, int indexCount, const XMMATRIX& worldMatrix, const XMMATRIX& lightViewMatrix, const XMMATRIX& lightOrthographicMatrix)
{
	return depthShader->Render(deviceContext, indexCount, worldMatrix, lightViewMatrix, lightOrthographicMatrix);
}

bool ShaderManager::RenderSkyDomeShader(ID3D11DeviceContext* deviceContext, int indexCount, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix, const XMFLOAT4& apexColour, const XMFLOAT4& centreColour)
{
	return skyDomeShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, apexColour, centreColour);
}
