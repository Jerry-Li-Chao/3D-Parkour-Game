//
//  Arrow.cpp
//  Lab11
//
//  Created by Apple on 2020/11/11.
//

#include "Arrow.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "Checkpoint.hpp"

Arrow::Arrow(Game* game, Actor* parent)
:Actor(game, parent)
{
    mc = new MeshComponent(this);
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Arrow.gpmesh"));
    SetScale(0.15f);
}


void Arrow::OnUpdate(float deltaTime)
{
    if(mGame->GetCheckpoint().empty()){
        setQuaternion(Quaternion::Identity);
    }
    else{
        Vector3 playertoCheckpoint = mGame->GetCheckpoint().front()->GetPosition() - GetPosition();
        playertoCheckpoint.Normalize();
        
        Vector3 unit {1.0f, 0.0f, 0.0f};
        float angle = acos(Vector3::Dot(unit, playertoCheckpoint));
        
        if(angle == 1.0f){
            setQuaternion(Quaternion::Identity);
        }
        else if (angle == -1.0f){
            setQuaternion(Quaternion(Vector3::UnitZ, Math::Pi));
        }
        else{
            Vector3 raw_axis = Vector3::Cross(unit, playertoCheckpoint);
            float raw_axis_length = raw_axis.Length();
            Vector3 axis {raw_axis.x / raw_axis_length, raw_axis.y / raw_axis_length, raw_axis.z / raw_axis_length};
            setQuaternion(Quaternion(axis, angle));
        }
    }
    
    //Use "Unprojection" to update the position
    Vector3 arrow_pos = mGame->GetRenderer()->Unproject(screenPoint);
    SetPosition(arrow_pos);
    
}
