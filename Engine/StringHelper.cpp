#include "StringHelper.h"
#include <algorithm>
#include <ctime>

std::wstring StringHelper::StringToWide(std::string str)
{
    std::wstring wide_string(str.begin(), str.end());
    return wide_string;
}

std::string StringHelper::GetDirectoryFromPath(const std::string & filepath)
{
    size_t off1 = filepath.find_last_of('\\');
    size_t off2 = filepath.find_last_of('/');
    if (off1 == std::string::npos && off2 == std::string::npos) //If no slash or backslash in path?
        {
        return "";
        }
    if (off1 == std::string::npos)
    {
        return filepath.substr(0, off2);
    }
    if (off2 == std::string::npos)
    {
        return filepath.substr(0, off1);
    }
    //If both exists, need to use the greater offset
    return filepath.substr(0, std::max(off1, off2));
}

std::string StringHelper::GetFileExtension(const std::string & filename)
{
    size_t off = filename.find_last_of('.');
    if (off == std::string::npos)
    {
        return {};
    }
    return std::string(filename.substr(off + 1));
}

std::wstring StringHelper::GetFileExtensionWide(const std::wstring & filename)
{
    size_t off = filename.find_last_of('.');
    if (off == std::wstring::npos)
    {
        return {};
    }
    return std::wstring(filename.substr(off + 1));
}

std::string StringHelper::ToString(const DirectX::XMFLOAT3 vector3)
{
    return "(" + std::to_string(vector3.x) + ", " + std::to_string(vector3.y) + ", " + std::to_string(vector3.z) + ")";
}

std::string StringHelper::ToString(const DirectX::XMFLOAT4 vector4)
{
    return "(" + std::to_string(vector4.x) + ", " + std::to_string(vector4.y) + ", " + std::to_string(vector4.z)  + ", " + std::to_string(vector4.w) + ")";
}

std::string StringHelper::ToString(const DirectX::XMVECTOR vector)
{
    DirectX::XMFLOAT4 vector4;
    DirectX::XMStoreFloat4(&vector4, vector);
    return ToString(vector4);
}

std::string StringHelper::ToString(const DirectX::XMMATRIX matrix)
{
    DirectX::XMVECTOR* outScale = new DirectX::XMVECTOR;
    DirectX::XMVECTOR* outRotQuat = new DirectX::XMVECTOR;
    DirectX::XMVECTOR* outTrans = new DirectX::XMVECTOR;
    DirectX::XMMatrixDecompose(outScale, outRotQuat, outTrans, matrix);

    return "Trans: " + ToString(*outTrans) + ", Rot: " + ToString(*outRotQuat) + ", Scale: " + ToString(*outScale);
}

std::string StringHelper::ToString(__time64_t& time)
{
    // Stores the current time into a struct.
    auto timeStruct = *std::localtime(&time);

    // Converts the current time to a char array in the Year-Month-Day Hour-Minute-Second format.
    char buffer[20];
    strftime(buffer, sizeof buffer, "%F %H-%M-%S\0", &timeStruct);

    // Converts the char array into a string.
    // 'sizeof buffer - 1' disregards the extra space at the end of the char array.
    return std::string(buffer, buffer + sizeof buffer - 1);
}
