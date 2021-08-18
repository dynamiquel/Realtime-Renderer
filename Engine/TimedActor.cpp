#include "TimedActor.h"

#include "Scene.h"


TimedActor::TimedActor(Node* parent) : Actor(parent)
{
}

TimedActor::TimedActor(std::string name, Node* parent) : Actor(name, parent)
{
}

void TimedActor::OnUpdate(float deltaTime)
{
    if ((timePassed += deltaTime) >= lifetime)
        GetScene()->DestroyNode(this);
}
