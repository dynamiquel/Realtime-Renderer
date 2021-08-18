#pragma once
#include <DirectXMath.h>
#include <string>

/**
 * \brief Contains useful methods for strings.
 */
class StringHelper
{
public:
    static std::wstring StringToWide(std::string str);
    static std::string GetDirectoryFromPath(const std::string& filepath);
    static std::string GetFileExtension(const std::string& filename);
    static std::wstring GetFileExtensionWide(const std::wstring& filename);
    static std::string ToString(const DirectX::XMFLOAT3 vector3);
    static std::string ToString(const DirectX::XMFLOAT4 vector4);
    static std::string ToString(const DirectX::XMVECTOR vector);
    static std::string ToString(const DirectX::XMMATRIX matrix);
    static std::string ToString(__time64_t& time);
};
