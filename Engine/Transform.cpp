#include "Transform.h"

#include <DirectXMath.h>

Transform::Transform()
{
    translation = DirectX::XMFLOAT3(0, 0, 0);
    rotation = DirectX::XMFLOAT4(0, 0, 0, 1);
    oRotation = DirectX::XMFLOAT3(0, 0, 0);
    parentAxisRotation = DirectX::XMFLOAT3(0, 0, 0);
    scale = DirectX::XMFLOAT3(1, 1, 1);
}

Transform::Transform(DirectX::XMFLOAT3 location)
{
    translation = location;
    rotation = DirectX::XMFLOAT4(0, 0, 0, 1);
    scale = DirectX::XMFLOAT3(1, 1, 1);
}

Transform::Transform(DirectX::XMFLOAT3 location, DirectX::XMFLOAT4 rotation, DirectX::XMFLOAT3 scale)
{
    this->translation = location;
    this->rotation = rotation;
    this->scale = scale;
}

Transform::Transform(float locationX, float locationY, float locationZ)
{
    Transform(DirectX::XMFLOAT3(locationX, locationY, locationZ));
}

DirectX::XMFLOAT3 Transform::GetRotationEuler()
{
   /* DirectX::XMVECTOR rotationEulerVector;
    float qAngle = 0.f;
    const DirectX::XMVECTOR quaternion = DirectX::XMVectorSet(
        rotation.x,
        rotation.y,
        rotation.z,
        rotation.w);

    DirectX::XMQuaternionToAxisAngle(&rotationEulerVector, &qAngle, quaternion);
    
    return DirectX::XMFLOAT3(
        DirectX::XMVectorGetX(rotationEulerVector),
        DirectX::XMVectorGetY(rotationEulerVector),
        DirectX::XMVectorGetZ(rotationEulerVector));*/
    return oRotation;
}

void Transform::SetLocation(DirectX::XMFLOAT3 newLocation)
{
    translation = newLocation;
}

void Transform::SetLocation(float x, float y, float z)
{
    SetLocation(DirectX::XMFLOAT3(x, y, z));
}

void Transform::AdjustLocation(DirectX::XMFLOAT3& offset)
{
    SetLocation(DirectX::XMFLOAT3(
        translation.x + offset.x,
        translation.y + offset.y,
        translation.z + offset.z));
}

void Transform::AdjustLocation(float x, float y, float z)
{
    AdjustLocation(DirectX::XMFLOAT3(x, y, z));
}

void Transform::SetRotation(DirectX::XMFLOAT3 newRotation)
{
    oRotation = newRotation;
    
    const DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationRollPitchYaw(
        newRotation.x,
        newRotation.y,
        newRotation.z);

    rotation = DirectX::XMFLOAT4(
        DirectX::XMVectorGetX(quaternion),
        DirectX::XMVectorGetY(quaternion),
        DirectX::XMVectorGetZ(quaternion),
        DirectX::XMVectorGetW(quaternion));
}

void Transform::SetRotation(float x, float y, float z)
{
    SetRotation(DirectX::XMFLOAT3(x, y, z));
}

void Transform::AdjustRotation(DirectX::XMFLOAT3& offset)
{
    SetRotation(DirectX::XMFLOAT3(
        oRotation.x + offset.x,
        oRotation.y + offset.y,
        oRotation.z + offset.z));
}

void Transform::AdjustRotation(float x, float y, float z)
{
    AdjustRotation(DirectX::XMFLOAT3(x, y, z));
}

void Transform::SetRotation(DirectX::XMFLOAT4 newQuaternion)
{
    rotation = newQuaternion;
}

void Transform::SetRotation(float x, float y, float z, float w)
{
    SetRotation(DirectX::XMFLOAT4(x, y, z, w));
}

void Transform::SetPRotation(DirectX::XMFLOAT3 newRotation)
{
    parentAxisRotation = newRotation;
}

void Transform::SetPRotation(float x, float y, float z)
{
    SetPRotation(DirectX::XMFLOAT3(x, y, z));
}

void Transform::SetScale(DirectX::XMFLOAT3 newScale)
{
    scale = newScale;
}

void Transform::SetScale(float x, float y, float z)
{
    SetScale(DirectX::XMFLOAT3(x, y, z));
}

Transform Transform::operator+(const Transform& other) const
{
    Transform newTransform = Transform();
    
    newTransform.SetLocation(
        translation.x + other.translation.x,
        translation.y + other.translation.y,
        translation.z + other.translation.z);

    newTransform.SetRotation(
        rotation.x + other.rotation.x,
        rotation.y + other.rotation.y,
        rotation.z + other.rotation.z,
        rotation.w + other.rotation.w);
    
    newTransform.SetRotation(
        oRotation.x + other.oRotation.x,
        oRotation.y + other.oRotation.y,
        oRotation.z + other.oRotation.z);

    newTransform.SetScale(
        scale.x * other.scale.x,
        scale.y * other.scale.y,
        scale.z * other.scale.z);

    return newTransform;
}

Transform Transform::operator*(const Transform& other) const
{
    Transform newTransform = Transform();
    
    newTransform.SetLocation(
        translation.x * other.translation.x,
        translation.y * other.translation.y,
        translation.z * other.translation.z);

    newTransform.SetRotation(
        rotation.x * other.rotation.x,
        rotation.y * other.rotation.y,
        rotation.z * other.rotation.z,
        rotation.w * other.rotation.w);
    
    newTransform.SetRotation(
        oRotation.x * other.oRotation.x,
        oRotation.y * other.oRotation.y,
        oRotation.z * other.oRotation.z);

    newTransform.SetScale(
        scale.x * other.scale.x,
        scale.y * other.scale.y,
        scale.z * other.scale.z);

    return newTransform;
}
