//
//  Player.cpp
//  Lab07
//
//  Created by Apple on 2020/10/7.
//

#include "Player.hpp"
#include "Game.h"
#include "CollisionComponent.h"
#include "Renderer.h"
#include "PlayerMove.hpp"
#include "CameraComponent.hpp"

Player::Player(Game* game, Actor* parent)
:Actor(game, parent)
{
    mPlayerMove = new PlayerMove(this);
    mCamera = new CameraComponent(this);
    mColli = new CollisionComponent(this);
    mColli->SetSize(50.0f, 175.0f, 50.0f);
}

PlayerMove* Player::getPlayerMove()
{
    return mPlayerMove;
}
