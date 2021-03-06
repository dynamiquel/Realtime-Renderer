////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Texture.h"


Texture::Texture()
{
	texture = 0;
}


Texture::Texture(const Texture& other)
{
}


Texture::~Texture()
{
}


bool Texture::Initialize(ID3D11Device* device, WCHAR* filename)
{
	HRESULT result;


	// Load the texture in.
	result = CreateDDSTextureFromFile(device, filename, NULL, &texture, NULL);

	if(FAILED(result))
	{
		return false;
	}

	return true;
}


void Texture::Release()
{
	// Release the texture resource.
	if(texture)
	{
		texture->Release();
		texture = nullptr;
	}
}