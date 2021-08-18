#include "Node.h"

#include "Scene.h"

Node::Node()
{
    // This should only be used for the World node (the actual root node).
    name = "World";
    parent = nullptr;
    creationTime = 0;
}

Node::Node(Node* parent)
{
    name = "";
    SetParent(parent);
    creationTime = 0;
}

Node::Node(std::string name, Node* parent)
{
    SetName(name);
    SetParent(parent);
    creationTime = 0;

    // If the given parent is a Scene, cache the scene. It's cheap enough to do so.
    auto* foundScene = dynamic_cast<Scene*>(GetParent());
    if (foundScene)
        scene = foundScene;
}

Node::~Node()
{
    
}

Scene* Node::GetScene()
{
    // If the cached world is null, attempt to find the world by finding the highest root.
    if (scene)
        return scene;

    if (!parent)
        return nullptr;

    // Checks if the parent is the scene.
    auto* foundScene = dynamic_cast<Scene*>(GetParent());
    if (foundScene)
    {
        // Caches the scene.
        scene = foundScene;
        return foundScene;
    }

    // Checks if another ancestor is the scene.
    foundScene = GetParent()->GetScene();
    if (foundScene)
    {
        // Caches the scene.
        scene = foundScene;
        return foundScene;
    }

    return nullptr;
}

Node* Node::GetRoot()
{
    // Scene node has no parent.
    // Only the Scene node should have no parent.
    // The direct children of the Scene node are considered Root nodes.
    // The Scene node is also considered a Root node.
   
    // Unless something is wrong, this means this is the Scene node.
    if (parent == nullptr)
        return this;

    // Unless something is wrong, this means this is a direct child of the Scene node.
    if (parent->GetParent() == nullptr)
        return this;

    // Recursion.
    return parent->GetRoot();
}

Node* Node::GetChild(int index)
{
    if (index >= children.size())
        return nullptr;
    
    return children[index];
}

bool Node::ContainsTag(const std::string tag)
{
    for (std::string* currentTag : tags)
        if (*currentTag == tag)
            return true;

    return false;
}

void Node::SetName(std::string& name)
{
    this->name = name;
}

void Node::AddChild(Node* child)
{
    if (!child)
    {
        printf("Child is null.");
        return;
    }

    // Add unique.
    if (std::find(children.begin(), children.end(), child) == children.end())
        children.push_back(child);
}

void Node::RemoveChild(Node* child)
{
    // Gets the index of the given child.
    // Remains as -1 if the given node isn't a child of this node.
    int indexOfChild = -1;
    for (int i = 0; i < children.size(); i++)
    {
        if (children[i] == child)
        {
            indexOfChild = i;
            break;
        }
    }

    if (indexOfChild > -1)
    {
        children.erase(children.begin() + indexOfChild);
        child->parent = nullptr;
    }
}

void Node::SetParent(Node* node)
{
    // Remove this reference from the current parent.
    if (parent)
        parent->RemoveChild(this);
    
    if (!node)
    {
        printf("Parent is null.");
        return;
    }
    
    parent = node;
    parent->AddChild(this);
}
