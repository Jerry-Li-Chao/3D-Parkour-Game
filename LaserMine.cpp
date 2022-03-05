//
//  LaserMine.cpp
//  Lab10
//
//  Created by Apple on 2020/11/3.
//

#include "LaserMine.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "LaserComponent.hpp"

LaserMine::LaserMine(Game* game, Actor* parent)
:Actor(game, parent)
{
    MeshComponent* mc = new MeshComponent(this);
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/LaserMine.gpmesh"));
    new LaserComponent(this, true);
}
