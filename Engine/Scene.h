#pragma once

#include "Actor.h"

/**
 * \brief A Scene is a type of Node with no root. All other Nodes must descend from a Scene Node.
 * \n Includes basic Node factory functionality.
 */
class Scene : public Actor
{
public:
    Scene();
    Scene(std::string name);
    ~Scene();

    Actor* CreateActor() { return CreateActor("", this); }
    Actor* CreateActor(std::string name) { return CreateActor(name, this); }
    Actor* CreateActor(std::string name, Node* parent);

    // The proper way to destroy a Node. This will ensure everything is deleted and de-referenced properly.
    void DestroyNode(Node* node);

    void OnRender(D3DClass* d3d, ShaderManager* shaderManager, DirectX::XMMATRIX& worldMatrix,
        DirectX::XMMATRIX& viewMatrix, DirectX::XMMATRIX& projectionMatrix, const DirectX::BoundingFrustum& frustum,
        Light* light, Camera* camera, bool depth, ID3D11ShaderResourceView* depthMapTexture) override {}

    void OnUpdate(float deltaTime) override {}

private:
    // Stores pointers to all nodes created by the Scene.
    vector<Node*> nodes;

    void DestroyChildren(Node* node);
};
