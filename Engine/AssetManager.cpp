#include "AssetManager.h"


#include "Logger.h"
#include "StringHelper.h"
#include "Texture.h"

std::map<const std::wstring, Asset*> AssetManager::database;

Asset* AssetManager::GetAsset(ID3D11Device* d3d, std::wstring& assetName)
{
    auto position = database.find(assetName);

    // Asset found in database (memory); returns a pointer to it.
    if (position != database.end())
    {
        // Since position stores a key value pair, return the second value (the asset).
        return position->second;
    }

    // Asset not found in database (memory).
    // Attempts to load the asset from disk and then store it in the database (memory).
    
    Logger::Log(LogSource::Assets, LogType::Log, "Asset could not be found.");
    
    std::wstring extension = StringHelper::GetFileExtensionWide(assetName);

    // Model
    if (extension == L"txt")
    {
        return nullptr;
    }

    // Texture
    if (extension == L"dds")
    {
        Logger::Log(LogSource::Assets, LogType::Log, "Attempting to load texture asset.");

        Asset* asset = LoadTexture(d3d, assetName);

        // Adds asset to database.
        if (asset)
            database[assetName] = asset;

        return asset;
    }

    // Asset could not be loaded.
    return nullptr;
}

void AssetManager::DeleteAsset(std::wstring& assetName)
{
    auto position = database.find(assetName);
    if(position == database.end())
    {
        //m_Texture->Shutdown();
        //delete database[assetName];
        position->second->Release();
        database.erase(assetName);
    }
}

Asset* AssetManager::LoadTexture(ID3D11Device* d3d, std::wstring& assetName)
{
    // Create the texture object.
    Texture* texture = new Texture;
    if(!texture)
    {
        return nullptr;
    }

    // Initialize the texture object.
    WCHAR* asArray = &assetName[0];
    
    if(!texture->Initialize(d3d, asArray))
    {
        return nullptr;
    }

    return texture;
}
