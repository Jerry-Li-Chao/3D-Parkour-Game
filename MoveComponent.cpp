#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner)
:Component(owner, 50)
,mAngularSpeed(0.0f)
,mForwardSpeed(0.0f)
{
	
}

void MoveComponent::Update(float deltaTime)
{
    mOwner->SetRotation(mOwner->GetRotation() + mAngularSpeed * deltaTime);
    Vector3 currPosition = mOwner->GetPosition();
    Vector3 facing = mOwner->GetForward();
    Vector3 nextPosition = currPosition + facing * mForwardSpeed * deltaTime;
    mOwner->SetPosition(nextPosition);

    // updates the position of the owner based on its right
    // vector, the strafe speed, and delta time
    mOwner->SetPosition(mOwner->GetPosition() + mOwner->GetRight() * mStrafeSpeed * deltaTime);
}
