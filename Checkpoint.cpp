//
//  Checkpoint.cpp
//  Lab11
//
//  Created by Apple on 2020/11/5.
//

#include "Checkpoint.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "Player.hpp"

Checkpoint::Checkpoint(Game* game, Actor* parent)
:Actor(game, parent)
{
    mc = new MeshComponent(this);
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Checkpoint.gpmesh"));
    cc = new CollisionComponent(this);
    cc->SetSize(25.0f, 25.0f, 25.0f);
    
    mGame->addCheckpoint(this);
}

void Checkpoint::OnUpdate(float deltaTime)
{
    if(isActive){
        mc->SetTextureIndex(0);
    }
    else{
        mc->SetTextureIndex(1);
    }
    
    CollisionComponent* playercc = mGame->GetPlayer()->GetComponent<CollisionComponent>();
    if(playercc->Intersect(cc) && this == mGame->GetCheckpoint().front()){
        if(mLevelString != ""){
            mGame->setNextLevel(mLevelString);
            Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Checkpoint.wav"), 0);
        }
    }
}

void Checkpoint::setLevel(std::string mLevel)
{
    mLevelString = mLevel;
}

