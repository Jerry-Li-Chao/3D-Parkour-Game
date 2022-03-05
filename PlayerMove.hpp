//
//  PlayerMove.hpp
//  Lab07
//
//  Created by Apple on 2020/10/9.
//

#ifndef PlayerMove_hpp
#define PlayerMove_hpp

#include <stdio.h>
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "Math.h"
#include <string>

enum class MoveState
{
    OnGround,
    Jump,
    Falling,
    WallClimb,
    WallRun
};

class PlayerMove : public MoveComponent
{
public:
    PlayerMove(class Actor* owner);
    ~PlayerMove();
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    void ChangeState(MoveState mMoveState);
    MoveState getMoveState();
    CollisionComponent::CollSide GetCurrentSide() {return CurrentSide;}
    void SetVelocity(Vector3 v) {mVelocity = v;}
    void SetPendingForce(Vector3 pf) {mPendingForces = pf;}
    //void SetlastcolorisWhite(bool b) {last_color_is_white = b;}
protected:
    void UpdateOnGround(float deltatime);
    void UpdateJump(float deltatime);
    void UpdateFalling(float deltatime);
    void UpdateWallClimb(float deltatime);
    void UpdateWallRun(float deltatime);
    
    bool CanWallClimb(CollisionComponent::CollSide cs);
    bool CanWallRun(CollisionComponent::CollSide cs);
    
    CollisionComponent::CollSide FixCollision(class CollisionComponent* self, class CollisionComponent* block);
    void PhysicsUpdate(float deltaTime);
    void AddForce(const Vector3& force);
    void FixXYVelocity();

private:
    MoveState mCurrentState;
    Vector3 mVelocity;
    Vector3 mAcceleration;
    Vector3 mPendingForces;
    float mMass = 1.0f;
    Vector3 mGravity {0.0f, 0.0f, -980.0f};
    Vector3 mJumpForce {0.0f, 0.0f, 35000.0f};
    bool curr_frame = false;
    bool last_frame = false;
    //bool last_color_is_white = true;
    const float friction = 0.9f;
    const float MAX_XY_VELOCITY = 400.0f;
    const float FORCE_MULTIPLIER = 700.0f;
    const float ANGULAR_DIVIDER = 500.0f;
    const float AUNGULAR_MULTIPLIER = 10.0f;
    const Vector3 WALL_CLIMB_FORCE {0.0f, 0.0f, 1800.0f};
    const Vector3 mWallRunForce {0.0f, 0.0f, 1200.0f};
    float mWallRunTimer = 0.0f;
    float mWallClimbTimer = 0.0f;
    const float WALLCLIMB_ANGLE = 0.8f;
    const float WALLRUN_ANGLE = 0.7f;
    const float SPEED = 350.0f;
    const float WALL_REACTION_FORCE = 700.0f;
    const float CONE_HALF_ANGLE = 0.52359f;
    const float CONE_HEIGHT = 300.0f;
    const float YELLOW_THRESHOLD = 2.0f;
    const float RED_THRESHOLD = 0.5f;
    const float RUNNING_SOUND_THRESHOLD = 50.0f;
    const float FALLEN_TO_DEATH = -750.0f;
    const float MAX_WALL_TIME = 0.4f;
    
    CollisionComponent::CollSide CurrentSide;
    int mRunningSFX;
    //int sd_channel = -1;
};

#endif /* PlayerMove_hpp */
