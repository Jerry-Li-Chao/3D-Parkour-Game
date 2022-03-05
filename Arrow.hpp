//
//  Arrow.hpp
//  Lab11
//
//  Created by Apple on 2020/11/11.
//

#ifndef Arrow_hpp
#define Arrow_hpp

#include <stdio.h>
#include "Actor.h"
#include "Math.h"
#include "Renderer.h"

class Arrow : public Actor
{
public:
    Arrow(class Game* game, Actor* parent);
    void OnUpdate(float deltaTime) override;
    
private:
    class MeshComponent* mc;
    const Vector3 screenPoint {0.0f, 250.0f, 0.1f};
};

#endif /* Arrow_hpp */
