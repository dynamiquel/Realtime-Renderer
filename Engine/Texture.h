////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTURE_H_
#define _TEXTURE_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11_1.h>
#include "DDSTextureLoader.h"
#include "Asset.h"

using namespace DirectX;


////////////////////////////////////////////////////////////////////////////////
// Class name: TextureClass
////////////////////////////////////////////////////////////////////////////////
class Texture : public Asset
{
public:
	Texture();
	Texture(const Texture&);
	~Texture();

	bool Initialize(ID3D11Device*, WCHAR*);
	void Release() override;

	ID3D11ShaderResourceView* GetTexture() const { return texture; }

private:
	ID3D11ShaderResourceView* texture;
};

#endif