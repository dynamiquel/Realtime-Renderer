#include "Actor.h"


#include "Camera.h"
#include "d3dclass.h"
#include "DirectXMathMatrix.inl"
#include "Light.h"
#include "Logger.h"
#include "MathHelper.h"
#include "ShaderManager.h"
#include "StringHelper.h"

Actor::Actor(Node* parent) : Node(parent)
{
    modelClass = new ModelClass;
    modelTransform = Transform();
}

Actor::Actor(std::string name, Node* parent) : Node(name, parent)
{
    modelClass = new ModelClass;
    modelTransform = Transform();
}

Actor::~Actor()
{
    if (modelClass)
    {
        modelClass->Shutdown();
        delete modelClass;
        modelClass = nullptr;
    }
}

Transform Actor::GetWorldTransform()
{
    if (GetParent() == nullptr)
    {
        return GetLocalTransform();
    }

    Actor* actor = static_cast<Actor*>(GetParent());

    if (actor != nullptr)
    {
        return GetLocalTransform() + actor->GetWorldTransform();
    }

    return GetLocalTransform();
}

void Actor::SetWorldTransform(Transform& transform)
{
}

void Actor::OnRender(D3DClass* d3d, ShaderManager* shaderManager, DirectX::XMMATRIX& worldMatrix,
                   DirectX::XMMATRIX& viewMatrix, DirectX::XMMATRIX& projectionMatrix, const DirectX::BoundingFrustum& frustum, Light* light, Camera* camera, bool depth, ID3D11ShaderResourceView* depthMapTexture)
{
    // Reset directional vectors.
    upVector = forwardVector = rightVector = XMFLOAT3(0.f, 1.f, 0.f);

    Transform transform = GetWorldTransform();
    Transform localTransform = GetLocalTransform();

    XMFLOAT3 location = transform.GetLocation();
    XMMATRIX translateMatrix = XMMatrixTranslation(
        location.x,
        location.y,
        location.z);

    /*XMFLOAT4 rotation = transform.GetRotation();
    XMVECTOR quaternion = XMVectorSet(
        (rotation.x),
        (rotation.y),
        (rotation.z),
        (rotation.w));
		
    XMMATRIX rotateMatrix = XMMatrixRotationQuaternion(quaternion);*/

    // Rotates the actor, using parent as origin.
    // Gets the parent's rotation.

    XMMATRIX rotateMatrixFromParent;
    XMMATRIX parentTranslateMatrix;

    Actor* parentActor = static_cast<Actor*>(GetParent());
    if (parentActor)
    {
        XMFLOAT3 rotationFromParent = parentActor->GetWorldTransform().GetRotationEuler();
        XMFLOAT3 rotationFromParentOffset = localTransform.GetPRotationEuler();

        rotateMatrixFromParent = XMMatrixRotationRollPitchYaw(
            XMConvertToRadians(rotationFromParent.x + rotationFromParentOffset.x),
            XMConvertToRadians(rotationFromParent.y + rotationFromParentOffset.y),
            XMConvertToRadians(rotationFromParent.z + rotationFromParentOffset.z));

        XMFLOAT3 parentLocation = parentActor->GetWorldTransform().GetLocation();
        parentTranslateMatrix = XMMatrixTranslation(parentLocation.x, parentLocation.y, parentLocation.z);
    }
    else
    {
        XMFLOAT3 rotationFromParentOffset = localTransform.GetPRotationEuler();
        rotateMatrixFromParent = XMMatrixRotationRollPitchYaw(
            XMConvertToRadians(rotationFromParentOffset.x),
            XMConvertToRadians(rotationFromParentOffset.y),
            XMConvertToRadians(rotationFromParentOffset.z));
    }

    // Rotates the actor, using itself as origin.
    XMFLOAT3 rotation = localTransform.GetRotationEuler();
    XMMATRIX rotateMatrix = XMMatrixRotationRollPitchYaw(
        XMConvertToRadians(rotation.x),
        XMConvertToRadians(rotation.y),
        XMConvertToRadians(rotation.z));

    // Gets the scale of the actor.
    XMFLOAT3 scale = transform.GetScale();
    XMMATRIX scaleMatrix = XMMatrixScaling(
        scale.x,
        scale.y,
        scale.z);

    /*  1. Rotates the actor on its own axis.
        2. Scales the actor.
        3. Translates the actor.
        4. Rotates the actor from the parent's axis.
    */
    //worldMatrix = scaleMatrix * rotateMatrix * translateMatrix * rotateMatrixFromParent;

    // Gets the location of the actor.
    XMFLOAT3 localLocation = localTransform.GetLocation();
    XMMATRIX re = XMMatrixTranslation(localLocation.x, localLocation.y, localLocation.z);

    // Scale.
    worldMatrix = scaleMatrix;
    
    // Rotate on its own axis.
    worldMatrix *= rotateMatrix;

    // Move to parent's origin.
    worldMatrix *= re;

    // Rotate on parent's axis.
    worldMatrix *= rotateMatrixFromParent;

    // Move to actual location.
    worldMatrix *= parentTranslateMatrix;

    // S * LR * LP * PR * PL =
    // 1. Moon is correct distance away from Earth.
    // 2. Moon orbits correctly.
    // 3. Moon does not follow Earth.
    // 4. Rotates same speed as Earth.
    // This may be fixed by changing how GetWorldTransform works so it gets the actor's location and position based
    // from world origin.

    // S * LR * P * PR =
    // 1. Moon is not correct distance from Earth.
    // 2. Moon does not orbit correctly (essentially orbits the Sun).
    // 3. Rotates same speed as Earth.

    // The world matrix contains the world transform of the Actor.
    // Splits the world matrix into scale, translation and rotation vectors that can be worked with.
    auto* outScale = new XMVECTOR;
    auto* outRotQuat = new XMVECTOR;
    auto* outTrans = new XMVECTOR;
    XMMatrixDecompose(outScale, outRotQuat, outTrans, worldMatrix);

    // Converts the vectors to their corresponding float counterparts.
    XMFLOAT3 trans;
    XMStoreFloat3(&trans, *outTrans);

    XMFLOAT4 rotQuat;
    XMStoreFloat4(&rotQuat, *outRotQuat);

    // GC
    delete outScale;
    delete outRotQuat;
    delete outTrans;

    // Gets the true extents of the Model instance by multiplying the Model's extents with the Actor's scale.
    XMFLOAT3 extents = MathHelper::MultiplyVector(GetModel()->GetExtents(), scale);

    // Creates a box that represents the cubic dimensions of the Model.
    auto modelCubicOutline = BoundingOrientedBox(trans, extents, rotQuat);
    
    // Render the Model if its cubic extents are contained within the frustum.
    if (frustum.Contains(modelCubicOutline))
    {
        GetModel()->Render(d3d->GetDeviceContext());
        
        if (depth)
        {
            if (!shaderManager->RenderDepthShader(d3d->GetDeviceContext(), GetModel()->GetIndexCount(), worldMatrix,
                                                      light->GetViewMatrix(), light->GetOrthographicMatrix()))
            {
                Logger::Log(LogSource::Rendering, LogType::Error, "Could not render the depth for: " + GetName());
            }

            return;
        }
        else
        {
            /*if (!shaderManager->RenderTextureShader(d3d->GetDeviceContext(), GetModel()->GetIndexCount(), worldMatrix,
            viewMatrix, projectionMatrix, GetModel()->GetTexture()))
            {
            Logger::Log(LogSource::Rendering, LogType::Error, "Could not render the model for: " + GetName());
            }*/
            
            
            if (!shaderManager->RenderLightShader(d3d->GetDeviceContext(), GetModel()->GetIndexCount(), worldMatrix,
                                                      viewMatrix, projectionMatrix, GetModel()->GetTexture(), light, camera->GetLocation()))
            {
                Logger::Log(LogSource::Rendering, LogType::Error, "Could not render the shader for: " + GetName());
            }
            if (!shaderManager->RenderShadowShader(d3d->GetDeviceContext(), GetModel()->GetIndexCount(), worldMatrix,
                                     viewMatrix, projectionMatrix, GetModel()->GetTexture(), depthMapTexture, light))
            {
                Logger::Log(LogSource::Rendering, LogType::Error, "Could not render the shadow shader for: " + GetName());
            }
        }
    }

    XMStoreFloat3(&forwardVector, XMVector3TransformCoord(XMVectorSet(0.f, 0.f, 1.f, 0.f), rotateMatrix));
    XMStoreFloat3(&rightVector, XMVector3TransformCoord(XMVectorSet(1.f, 0.f, 0.f, 0.f), rotateMatrix));
    XMStoreFloat3(&upVector, XMVector3TransformCoord(XMVectorSet(0.f, 1.f, 0.f, 0.f), rotateMatrix));
}
