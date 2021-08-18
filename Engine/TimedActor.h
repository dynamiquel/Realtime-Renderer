#pragma once
#include "Actor.h"

/**
 * \brief Just a basic Actor that has a lifetime.
 */
class TimedActor : public Actor
{
public:
    TimedActor(Node* parent);
    TimedActor(std::string name, Node* parent);

public:
   float lifetime = 8.f;

public:
    void OnUpdate(float deltaTime) override;

private:
    float timePassed = 0.f;
};