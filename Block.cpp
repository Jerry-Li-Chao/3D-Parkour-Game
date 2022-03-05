//
//  Block.cpp
//  Lab09
//
//  Created by Apple on 2020/10/22.
//

#include "Block.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "CollisionComponent.h"

Block::Block(Game* game, Actor* parent)
:Actor(game, parent)
{
    SetScale(64.0f);
    mc = new MeshComponent(this);
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
    mColli = new CollisionComponent(this);
    mColli->SetSize(1.0f, 1.0f, 1.0f);
    
    mGame->AddBlock(this);
}

Block::~Block()
{
    mGame->RemoveBlock(this);
}

void Block::OnUpdate(float deltaTime)
{
    if(rotates){
        float rotation = GetRotation() + (Math::PiOver2 / 2.0f) * deltaTime;
        SetRotation(rotation);
    }
}
