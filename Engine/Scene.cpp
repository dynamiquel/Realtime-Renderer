#include "Scene.h"

#include <queue>

Scene::Scene() : Actor("World", nullptr)
{
    
}

Scene::Scene(std::string name) : Actor(name, nullptr)
{
}

Scene::~Scene()
{
}

Actor* Scene::CreateActor(std::string name, Node* parent)
{
    auto* actor = new Actor(name, parent);
    nodes.push_back(actor);

    return actor;
}

void Scene::DestroyNode(Node* node)
{
    // Creates a queue and pushes the Scene (root) node to it.
    queue<Node*> queue;
    queue.push(this);

    //Logger::Log(LogSource::Camera, LogType::Log, StringHelper::ToString(m_Camera->GetFrustum().Orientation));

    // Renders everything in the Scene hierarchy using a breadth-first approach.
    while (queue.size() > 0)
    {
        // Gets the next node in the queue.
        Node* currentNode = queue.front();
        // Removes this node from the queue since it doesn't need to be rendered again.
        queue.pop();

        if (node == currentNode)
        {
            // Removes reference of this node from the parent.
            node->GetParent()->RemoveChild(node);
            // Destroys all the children of this node.
            DestroyChildren(node);
            // Deletes this node.
            delete node;
            node = nullptr;
            return;
        }

        // Adds the children of the node to the end of the queue.
        for (Node* childNode : currentNode->GetChildren())
            queue.push(childNode);
    }
}

void Scene::DestroyChildren(Node* node)
{
    // Deletes all the children.
    for (Node* childNode : node->GetChildren())
    {
        DestroyChildren(childNode);
        delete childNode;
        childNode = nullptr;
    }
}
