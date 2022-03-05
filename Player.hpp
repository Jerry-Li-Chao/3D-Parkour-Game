//
//  Player.hpp
//  Lab07
//
//  Created by Apple on 2020/10/7.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include "Actor.h"
#include "Math.h"

class Player : public Actor
{
public:
    Player(class Game* game, Actor* parent);
    class PlayerMove* getPlayerMove();
    void SetRespawnPos(Vector3 respawn_pos) {RESPAWN = respawn_pos;}
    Vector3 GetRespawnPos() {return RESPAWN;}
private:
    class PlayerMove* mPlayerMove;
    class CameraComponent* mCamera;
    class CollisionComponent* mColli;
    const float WIDTH = 100.0f;
    const float HEIGHT = 40.0f;
    const float DEPTH = 60.0f;
    Vector3 RESPAWN;
};


#endif /* Player_hpp */
