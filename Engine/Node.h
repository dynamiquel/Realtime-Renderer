#pragma once
#include <directxcollision.h>
#include <DirectXMath.h>
#include <string>
#include <vector>

#include "DDSTextureLoader.h"

class Camera;
class Light;
class ShaderManager;
class D3DClass;
class Scene;

/**
 * \brief A Node is the most basic 'in-game' object. All in-game objects should be Nodes as it provides a basic
 * tree format that can be searched and looped by the renderer.
 * \n All nodes, except Scene nodes should have a parent.
 */
class Node
{
public:
    Node();
    Node(Node* parent);
    Node(std::string name, Node* parent);
    virtual ~Node();

public:
    // False: disables this node and all of its children from being processed by the renderer.
    bool enabled = true;

private:
    // Non-unique name of the Node.
    std::string name;
    
    // Parent of the Node.
    Node* parent = nullptr;
    
    // Children of the Node.
    std::vector<Node*> children;
    
    bool beingDestroyed = false;
    
    // The time this Node was created (in seconds from renderer start).
    float creationTime;
    
    // Tags assigned to the Node. Tags can be used to implement certain functionality to specific Nodes.
    std::vector<std::string*> tags;
    
    // Cached pointer to the Scene owning the Node.
    Scene* scene = nullptr;

public:
    std::string& GetName() { return name; }
    
    // Returns the parent of the Node.
    Node* GetParent() const { return parent; }
    
    // Returns the Scene Node that the Node belongs to.
    Scene* GetScene();
    
    // Returns the top-most parent of the Node (excl. the Scene Node).
    Node* GetRoot();

    // Returns the direct descendants of the Node.
    std::vector<Node*> GetChildren() const { return children; }

    // Returns the direct descendant of this Node by the given index.
    Node* GetChild(int index);

    // Returns the tags of the Node.
    std::vector<std::string*> GetTags() { return tags;}

    // Returns true if the Node has the given tag.
    bool ContainsTag(const std::string tag);

    // Not yet implemented.
    bool GetBeingDestroyed() const { return beingDestroyed; }

    // Not yet implemented.
    float GetCreationTime() const { return creationTime; }

    void SetName(std::string& name);

    // Sets the given Node as a child of this Node.
    void AddChild(Node* child);

    // Removes the given Node as a direct descendant of the Node (if it exists).
    void RemoveChild(Node* child);

    // Called once per frame. Override to implement functionality.
    // Maybe a bit lazy but it just sends everything you could possibly need for rendering.
    virtual void OnRender(D3DClass* d3d, ShaderManager* shaderManager, DirectX::XMMATRIX& worldMatrix, DirectX::XMMATRIX& viewMatrix,
                        DirectX::XMMATRIX& projectionMatrix, const DirectX::BoundingFrustum& frustum, Light* light, Camera* camera, bool depth, ID3D11ShaderResourceView* depthMapTexture) = 0;

    virtual void OnUpdate(float deltaTime) = 0;


protected:
    void SetParent(Node* node);
};
