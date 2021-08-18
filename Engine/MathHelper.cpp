#include "MathHelper.h"

DirectX::XMFLOAT3 MathHelper::MultiplyVector(const DirectX::XMFLOAT3& cur, float other)
{
    return DirectX::XMFLOAT3(cur.x * other, cur.y * other, cur.z * other);
}

DirectX::XMFLOAT3 MathHelper::MultiplyVector(const DirectX::XMFLOAT3& cur, const DirectX::XMFLOAT3& other)
{
    return DirectX::XMFLOAT3(cur.x * other.x, cur.y * other.y, cur.z * other.z);
}

DirectX::XMFLOAT3 MathHelper::NegateVector(const DirectX::XMFLOAT3& cur)
{
    return DirectX::XMFLOAT3(-cur.x, -cur.y, -cur.z);
}

DirectX::XMFLOAT3 MathHelper::AddVector(const DirectX::XMFLOAT3& cur, const DirectX::XMFLOAT3& other)
{
    return DirectX::XMFLOAT3(cur.x + other.x, cur.y + other.y, cur.z + other.z);
}
