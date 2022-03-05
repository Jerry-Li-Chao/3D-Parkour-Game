//
//  SecurityCone.cpp
//  Lab11
//
//  Created by Apple on 2020/11/12.
//

#include "SecurityCone.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"

SecurityCone::SecurityCone(Game* game, Actor* parent)
:Actor(game, parent)
{
    mc = new MeshComponent(this, true);
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cone.gpmesh"));
    Vector3 pos {30.0f, 0.0f, 0.0f};
    SetPosition(pos);
    SetScale(3.0f);
    mGame->AddCone(this);
}

SecurityCone::~SecurityCone()
{
    mGame->RemoveCone(this);
}
