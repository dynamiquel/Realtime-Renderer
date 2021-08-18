#pragma once
#include <DirectXMath.h>
#include "axis.h"

/**
 * \brief Responsible for storing positional, rotational and scaling data.
 */
struct Transform
{
public:
    Transform();
    Transform(DirectX::XMFLOAT3 location);
    Transform(DirectX::XMFLOAT3 location, DirectX::XMFLOAT4 rotation, DirectX::XMFLOAT3 scale);
    Transform(float locationX, float locationY, float locationZ);

private:
    DirectX::XMFLOAT3 translation;
    // Not yet implemented.
    // Rotation represented as a quaternion.
    DirectX::XMFLOAT4 rotation;
    DirectX::XMFLOAT3 oRotation;
    DirectX::XMFLOAT3 parentAxisRotation;
    DirectX::XMFLOAT3 scale;

public:
    // Returns the location.
    DirectX::XMFLOAT3 GetLocation() { return translation; }
    
    // Returns the rotation as a quaternion.
    DirectX::XMFLOAT4 GetRotation() { return rotation; }
    // Returns the rotation as a 3D vector.
    DirectX::XMFLOAT3 GetRotationEuler();

    // Returns the rotation from the parent as a 3D vector.
    DirectX::XMFLOAT3 GetPRotationEuler() { return parentAxisRotation; }
    
    // Returns the scale.
    DirectX::XMFLOAT3 GetScale() { return scale; }

    // Not yet implemented.
    DirectX::XMFLOAT3 GetForwardVector();
    // Not yet implemented.
    DirectX::XMFLOAT3 GetRightVector();
    // Not yet implemented.
    DirectX::XMFLOAT3 GetUpVector();

    // Sets the location with the given 3D vector.
    void SetLocation(DirectX::XMFLOAT3 newLocation);
    // Sets the location with the given dimensions.
    void SetLocation(float x, float y, float z);

    // Adjusts the location with the given 3D vector.
    void AdjustLocation(DirectX::XMFLOAT3& offset);
    // Adjusts the location with the given dimensions.
    void AdjustLocation(float x, float y, float z);

    // Sets the rotation with the given 3D vector.
    void SetRotation(DirectX::XMFLOAT3 newRotation);
    // Sets the rotation with the given three dimensions.
    void SetRotation(float x, float y, float z);

    // Adjusts the rotation with the given 3D vector.
    void AdjustRotation(DirectX::XMFLOAT3& offset);
    // Adjusts the rotation with the given three dimensions.
    void AdjustRotation(float x, float y, float z);
    
    // Not yet implemented.
    // Sets the rotation with the given quaternion.
    void SetRotation(DirectX::XMFLOAT4 newQuaternion);
    // Sets the rotation with the given four dimensions.
    void SetRotation(float x, float y, float z, float w);

    // Sets the rotation from the parent axis with the given 3D vector.
    void SetPRotation(DirectX::XMFLOAT3 newRotation);
    // Sets the rotation from the parent with the given three dimensions.
    void SetPRotation(float x, float y, float z);
    
    // Sets the scale with the given 3D vector.
    void SetScale(DirectX::XMFLOAT3 newScale);
    // Sets the scale with the given dimensions.
    void SetScale(float x, float y, float z);
    // Scales the X, Y and Z dimensions with the given value.
    void SetScale(float scale) { SetScale(scale, scale, scale); }

    // Not yet implemented.
    // Flips the transform by the given Axis.
    void Flip(Axis flipAxis);

    Transform& operator==(const Transform& other);
    Transform operator+(const Transform& other) const;
    Transform& operator+=(const Transform& other);
    Transform operator*(const Transform& other) const;
};
