#pragma once
#include <d3d11.h>
#include <map>
#include <string>


#include "Asset.h"

/**
 * \brief Responsible for caching assets that do not need to instantiated.
 * This reduces memory footprint since it means there are not unnecessary duplicates.
 * So far this includes: textures.
 */
class AssetManager
{
public:
    /**
     * \brief Returns the asset with the given file name. Will return nullptr if no asset could be found.
     */
    static Asset* GetAsset(ID3D11Device* d3d, std::wstring& assetName);

    /**
     * \brief Deletes the asset with the given file name from memory.
     */
    static void DeleteAsset(std::wstring& assetName);

private:
    /**
     * \brief Stores all the assets in a dictionary. Filename of the asset is used as the key.
     */
    static std::map<const std::wstring, Asset*> database;

    /**
     * \brief Attempts to load a texture from disk.
     */
    static Asset* LoadTexture(ID3D11Device* d3d, std::wstring& assetName);

/* Assume:
 * - txt files are models
 * - dds files are textures
 */
};
