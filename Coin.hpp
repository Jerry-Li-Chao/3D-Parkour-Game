//
//  Coin.hpp
//  Lab11
//
//  Created by Apple on 2020/11/11.
//

#ifndef Coin_hpp
#define Coin_hpp

#include <stdio.h>
#include "Actor.h"

class Coin : public Actor
{
public:
    Coin(class Game* game, Actor* parent);
    void OnUpdate(float deltaTime) override;
private:
    class MeshComponent* mc;
    class CollisionComponent* cc;
};

#endif /* Coin_hpp */
