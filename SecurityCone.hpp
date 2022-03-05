//
//  SecurityCone.hpp
//  Lab11
//
//  Created by Apple on 2020/11/12.
//

#ifndef SecurityCone_hpp
#define SecurityCone_hpp

#include <stdio.h>
#include "Actor.h"

class SecurityCone : public Actor
{
public:
    SecurityCone(class Game* game, Actor* parent);
    ~SecurityCone();
    float GetYellowTimer() {return yellow_timer;}
    float GetRedTimer() {return red_timer;}
    void SetYellowTimer (float y) {yellow_timer = y;}
    void SetRedTimer (float r) {red_timer = r;}
private:
    class MeshComponent* mc;
    float yellow_timer = 0.0f;
    float red_timer = 0.0f;
};

#endif /* SecurityCone_hpp */
