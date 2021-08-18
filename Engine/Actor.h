#pragma once
#include "Node.h"
#include "Transform.h"
#include "modelclass.h"

class Camera;
/**
 * \brief A Node with 3D positional and rendering capabilities.
 */
class Actor : public Node
{
public:
    Actor(Node* parent);
    Actor(std::string name, Node* parent);
    virtual ~Actor();

public:
    // Not yet implemented.
    /**
     * \brief Enabled: Saves performance but limits the movement capabilities of the Actor.
     * \n Keep disabled unless you are certain the Actor is not going to move.
     */
    bool stationary = false;

    // Not yet implemented.
    // Enabled: Prevents the Actor's X-axis location from being changed.
    bool lockX = false;
    // Enabled: Prevents the Actor's Y-axis location from being changed.
    bool lockY = false;
    // Enabled: Prevents the Actor's Z-axis location from being changed.
    bool lockZ = false;

private:
    // Model transform of the Actor.
    Transform modelTransform;
    ModelClass* modelClass;

protected:
    XMFLOAT3 forwardVector;
    XMFLOAT3 upVector;
    XMFLOAT3 rightVector;

public:
    // Returns the transform as an offset from the parent (model transform).
    Transform& GetLocalTransform() { return modelTransform; }

    XMFLOAT3 GetForwardVector() const { return forwardVector; }
    XMFLOAT3 GetUpVector() const { return upVector; }
    XMFLOAT3 GetRightVector() const { return rightVector; }

    // Equivalent to GetLocalTransform().GetLocation().
    XMFLOAT3 GetLocation() { return GetLocalTransform().GetLocation(); }
    // Equivalent to GetLocalTransform().GetRotation().
    XMFLOAT4 GetRotation() { return GetLocalTransform().GetRotation(); }
    // Equivalent to GetLocalTransform().GetScale().
    XMFLOAT3 GetScale() { return GetLocalTransform().GetScale(); }

    // Equivalent to GetLocalTransform().SetLocation().
    void SetLocation(XMFLOAT3 newLocation) { GetLocalTransform().SetLocation(newLocation); }
    // Equivalent to GetLocalTransform().SetRotation().
    void SetRotation(XMFLOAT4 newRotation) { GetLocalTransform().SetRotation(newRotation); }
    // Equivalent to GetLocalTransform().SetRotation().
    void SetRotation(XMFLOAT3 newRotation) { GetLocalTransform().SetRotation(newRotation); }
    // Equivalent to GetLocalTransform().SetScale().
    void SetScale(XMFLOAT3 newScale) { GetLocalTransform().SetScale(newScale); }
    // Equivalent to GetLocalTransform().SetScale().
    void SetScale(float newUniformScale) { GetLocalTransform().SetScale(newUniformScale); }

    // Equivalent to GetLocalTransform().AdjustLocation().
    void AdjustLocation(XMFLOAT3 offset) { GetLocalTransform().AdjustLocation(offset); }
    // Equivalent to GetLocalTransform().AdjustRotation().
    void AdjustRotation(XMFLOAT3 offset) { GetLocalTransform().AdjustRotation(offset); }
    
    /**
     * \brief Returns the transform of the Actor as an offset from the Scene.
     * \n Adjustments to this Transform are not automatically applied to the Node. You must use
     * Actor::SetWorldTransform() to apply the changes.
     */
    Transform GetWorldTransform();

    // Not yet implemented.
    /**
     * \brief Adjusts the Transform of this object to the given world Transform.
     * \n Use GetLocalTransform() to adjust the transform where possible as it is cheaper.
     */
    void SetWorldTransform(Transform& transform);

    // TODO Refactor this thing. No like.
    ModelClass* GetModel() const { return modelClass; }
    
    void OnRender(D3DClass* d3d, ShaderManager* shaderManager, DirectX::XMMATRIX& worldMatrix,
                DirectX::XMMATRIX& viewMatrix, DirectX::XMMATRIX& projectionMatrix, const DirectX::BoundingFrustum& frustum, Light* light, Camera* camera, bool depth, ID3D11ShaderResourceView* depthMapTexture) override;

    void OnUpdate(float deltaTime) override {}
};
