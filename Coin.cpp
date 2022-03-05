//
//  Coin.cpp
//  Lab11
//
//  Created by Apple on 2020/11/11.
//

#include "Coin.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "Player.hpp"
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

Coin::Coin(Game* game, Actor* parent)
:Actor(game, parent)
{
    mc = new MeshComponent(this);
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Coin.gpmesh"));
    cc = new CollisionComponent(this);
    cc->SetSize(100.0f, 100.0f, 100.0f);
}

void Coin::OnUpdate(float deltaTime)
{
    float new_rot = GetRotation() + Math::Pi * deltaTime;
    SetRotation(new_rot);
    
    Player* mPlayer = mGame->GetPlayer();
    CollisionComponent* Playercc = mPlayer->GetComponent<CollisionComponent>();
    CollisionComponent* mcc = GetComponent<CollisionComponent>();
    if(Playercc->Intersect(mcc)){
        Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Coin.wav"), 0);
        SetState(ActorState::Destroy);
    }
}
