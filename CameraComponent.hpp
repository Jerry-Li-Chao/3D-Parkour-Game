//
//  CameraComponent.hpp
//  Lab08
//
//  Created by Apple on 2020/10/17.
//

#ifndef CameraComponent_hpp
#define CameraComponent_hpp

#include <stdio.h>
#include "Component.h"
#include "Math.h"
#include "CollisionComponent.h"

class CameraComponent : public Component
{
public:
    CameraComponent(class Actor* owner);
    void Update(float deltaTime) override;
    void SetPitchSpeed (float pSpeed) {mPitchSpeed = pSpeed;}
    float GetPitchSpeed () {return mPitchSpeed;}
private:
    float target_offset = 50.0f;
    float mPitchAngle = 0.0f;
    float mPitchSpeed = 0.0f;
    float WallRuntimer = 0.0f;
    float timer = 0.0f;
    const float TRANSITION = 0.4f;
    CollisionComponent::CollSide mCollSide = CollisionComponent::CollSide::None;
};

#endif /* CameraComponent_hpp */
