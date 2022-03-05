//
//  Checkpoint.hpp
//  Lab11
//
//  Created by Apple on 2020/11/5.
//

#ifndef Checkpoint_hpp
#define Checkpoint_hpp

#include <stdio.h>
#include "Actor.h"
#include <string>

class Checkpoint : public Actor
{
public:
    Checkpoint(class Game* game, Actor* parent);
//    ~Checkpoint();
    void OnUpdate(float deltaTime) override;
    void setState(bool s) {isActive = s;}
    bool getState() {return isActive;}
    void setLevel(std::string mLevel);
    void setNextLevel (std::string nextLevel);
    
private:
    class MeshComponent* mc;
    class CollisionComponent* cc;
    bool isActive = false;
    std::string mLevelString = "";
    
};

#endif /* Checkpoint_hpp */
