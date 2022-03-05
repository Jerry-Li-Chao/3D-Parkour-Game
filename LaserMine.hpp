//
//  LaserMine.hpp
//  Lab10
//
//  Created by Apple on 2020/11/3.
//

#ifndef LaserMine_hpp
#define LaserMine_hpp

#include <stdio.h>
#include "Actor.h"

class LaserMine : public Actor
{
public:
    LaserMine(class Game* game, Actor* parent);
};

#endif /* LaserMine_hpp */
