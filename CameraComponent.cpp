//
//  CameraComponent.cpp
//  Lab08
//
//  Created by Apple on 2020/10/17.
//

#include "CameraComponent.hpp"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "PlayerMove.hpp"
#include <iostream>


CameraComponent::CameraComponent(Actor* owner)
:Component(owner)
{
    
}


void CameraComponent::Update(float deltaTime)
{
    timer = 0.0f;
    mPitchAngle += mPitchSpeed * deltaTime;
    if(mPitchAngle > Math::PiOver2 / 2.0f){
        mPitchAngle = Math::PiOver2 / 2.0f;
    }
    else if(mPitchAngle < -Math::PiOver2 / 2.0f){
        mPitchAngle = -Math::PiOver2 / 2.0f;
    }
    
    Matrix4 pitch = Matrix4::CreateRotationY(mPitchAngle);
    Matrix4 yaw = Matrix4::CreateRotationZ(mOwner->GetRotation());
    Matrix4 combined = pitch * yaw;
    Vector3 unit {1, 0, 0};
    Vector3 forward = Vector3::Transform(unit, combined);
    
    Vector3 TargetPos = mOwner->GetPosition() + forward * target_offset;
    //rotate up-vector
    //(1) am I wallrunning?
    Vector3 offset;
    Matrix4 roll;
    Vector3 upVector = Vector3::UnitZ;
    PlayerMove* pm = mOwner->GetComponent<PlayerMove>();
    
    if(pm->getMoveState() == MoveState::WallRun)
    {
        WallRuntimer += deltaTime;
        if(WallRuntimer >= TRANSITION) {WallRuntimer = TRANSITION;}
        
        mCollSide = pm->GetCurrentSide();
        //xz-plane
        if(mCollSide == CollisionComponent::CollSide::SideMinY){
            roll = Matrix4::CreateRotationX(Math::Lerp(0.0f, Math::PiOver2 / 2.0f, WallRuntimer/TRANSITION));
        }
        //xz-plane
        else if(mCollSide == CollisionComponent::CollSide::SideMaxY){
            roll = Matrix4::CreateRotationX(Math::Lerp(0.0f, -Math::PiOver2 / 2.0f, WallRuntimer/TRANSITION));
        }
        //yz-plane
        else if(mCollSide == CollisionComponent::CollSide::SideMinX){
            roll = Matrix4::CreateRotationY(Math::Lerp(0.0f, -Math::PiOver2 / 2.0f, WallRuntimer/TRANSITION));
        }
        //yz-plane
        else if(mCollSide == CollisionComponent::CollSide::SideMaxX){
            roll = Matrix4::CreateRotationY(Math::Lerp(0.0f, Math::PiOver2 / 2.0f, WallRuntimer/TRANSITION));
        }
        upVector = Vector3::Transform(Vector3::UnitZ, roll);

    }
    else{
        WallRuntimer -= deltaTime;
        if(WallRuntimer <= 0.0f) {WallRuntimer = 0.0f;}
        //xz-plane
        if(mCollSide == CollisionComponent::CollSide::SideMinY){
            roll = Matrix4::CreateRotationX(Math::Lerp(0.0f, Math::PiOver2 / 2.0f, WallRuntimer/TRANSITION));
        }
        //xz-plane
        else if(mCollSide == CollisionComponent::CollSide::SideMaxY){
            roll = Matrix4::CreateRotationX(Math::Lerp(0.0f, -Math::PiOver2 / 2.0f, WallRuntimer/TRANSITION));
        }
        //yz-plane
        else if(mCollSide == CollisionComponent::CollSide::SideMinX){
            roll = Matrix4::CreateRotationY(Math::Lerp(0.0f, -Math::PiOver2 / 2.0f, WallRuntimer/TRANSITION));
        }
        //yz-plane
        else if(mCollSide == CollisionComponent::CollSide::SideMaxX){
            roll = Matrix4::CreateRotationY(Math::Lerp(0.0f, Math::PiOver2 / 2.0f, WallRuntimer/TRANSITION));
        }
        upVector = Vector3::Transform(Vector3::UnitZ, roll);
    }
    
    //put in actual camera position
    Matrix4 view = Matrix4::CreateLookAt( mOwner->GetPosition(), TargetPos, upVector);
    Renderer* mRenderer = mOwner->GetGame()->GetRenderer();
    mRenderer->SetViewMatrix(view);
    
    
}




