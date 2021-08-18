#pragma once
#include <DirectXMath.h>

/**
 * \brief Contains useful methods for math.
 */
class MathHelper
{
public:
    static DirectX::XMFLOAT3 MultiplyVector(const DirectX::XMFLOAT3& cur, float other);
    static DirectX::XMFLOAT3 MultiplyVector(const DirectX::XMFLOAT3& cur, const DirectX::XMFLOAT3& other);
    static DirectX::XMFLOAT3 NegateVector(const DirectX::XMFLOAT3& cur);
    static DirectX::XMFLOAT3 AddVector(const DirectX::XMFLOAT3& cur, const DirectX::XMFLOAT3& other);
};
