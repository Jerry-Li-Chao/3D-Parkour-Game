#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>
#include <math.h>

Actor::Actor(Game* game, Actor* parent)
	:mGame(game)
	,mState(ActorState::Active)
	,mPosition(Vector3::Zero)
	,mScale(1.0f)
	,mRotation(0.0f)
    ,mParent(parent)
{
    if(mParent == nullptr)
    {
        mGame->AddActor(this);
    }
    else{
        mParent->AddChild(this);
    }
    
}

Actor::~Actor()
{
    while (!mChildren.empty()) {
//        Actor* to_delete = mChildren.back();
//        //mChildren.pop_back();
//        delete to_delete;
        RemoveChild(mChildren.back());
    }
    for(auto i : mComponents){
        delete i;
    }
    if(mParent == nullptr)
    {
        mGame->RemoveActor(this);
    }
    else{
        mParent->RemoveChild(this);
    }
    

}

Vector3 Actor::GetForward(){
    Vector3 forwardDirection {cos(mRotation), sin(mRotation), 0.0f};
    return forwardDirection;
}

Vector3 Actor::GetRight()
{
    Vector3 Right{cos(mRotation + Math::PiOver2), sin(mRotation + Math::PiOver2), 0.0f};
    return Right;
}

void Actor::Update(float deltaTime)
{
    CalcWorldTransform();
    if(mState == ActorState::Active){
        for (auto c : mComponents){
            c->Update(deltaTime);
        }
        OnUpdate(deltaTime);
    }
    //mWorldTransform = mScale * mRotation * mPosition;
    CalcWorldTransform();
    
    for(Actor* c : mChildren){
        c->Update(deltaTime);
    }
}

void Actor::OnUpdate(float deltaTime)
{
}

void Actor::ProcessInput(const Uint8* keyState)
{
    if(mState == ActorState::Active){
        for(auto c : mComponents){
            c->ProcessInput(keyState);
        }
        OnProcessInput(keyState);
    }
}

void Actor::OnProcessInput(const Uint8* keyState)
{
}

void Actor::AddComponent(Component* c)
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
	});
}

Quaternion Actor::getQuaternion()
{
    return mQ;
}

void Actor::setQuaternion(Quaternion q)
{
    mQ = q;
}

Vector3 Actor::GetQuatForward()
{
    Vector3 forward = Vector3::Transform(Vector3::UnitX, mQ);
    forward.Normalize();
    
    return forward;
}

void Actor::CalcWorldTransform()
{
    mWorldTransform = Matrix4::CreateScale(mScale) * Matrix4::CreateRotationZ(mRotation)
    * Matrix4::CreateFromQuaternion(mQ) * Matrix4::CreateTranslation(mPosition);
    if(mParent != nullptr){
        if(mInheritScale){
            mWorldTransform *= mParent->mWorldTransform;
        }
        else{
            mWorldTransform *= mParent->CalcWorldRotTrans();
        }
    }
}

Matrix4 Actor::CalcWorldRotTrans()
{
    Matrix4 temp = Matrix4::CreateRotationZ(mRotation) * Matrix4::CreateFromQuaternion(mQ)
    * Matrix4::CreateTranslation(mPosition);
    if(mParent){
        temp *= mParent->CalcWorldRotTrans();
    }
    
    return temp;
}

Vector3 Actor::GetWorldPosition()
{
    return mWorldTransform.GetTranslation();
}

Vector3 Actor::GetWorldForward()
{
    return mWorldTransform.GetXAxis();
}

void Actor::RemoveChild(Actor* remove)
{
    auto it = std::find(mChildren.begin(), mChildren.end(), remove);
    if(it != mChildren.end())
    {
        mChildren.erase(it);
    }
}
