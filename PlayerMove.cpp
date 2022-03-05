//
//  PlayerMove.cpp
//  Lab07
//
//  Created by Apple on 2020/10/9.
//

#include "PlayerMove.hpp"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include <fstream>
#include <iostream>
#include "Random.h"
#include "MoveComponent.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "CameraComponent.hpp"
#include "Block.hpp"
#include <iostream>
#include "Player.hpp"
#include "Checkpoint.hpp"
#include "SecurityCone.hpp"
#include "SecurityCamera.hpp"
#include "MeshComponent.h"

PlayerMove::PlayerMove(Actor* owner)
:MoveComponent(owner)
{
    ChangeState(MoveState::Falling);
    mRunningSFX = Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Running.wav"), -1);
    Mix_Pause(mRunningSFX);
}

PlayerMove::~PlayerMove()
{
    Mix_HaltChannel(mRunningSFX);
}


void PlayerMove::Update(float deltaTime)
{
    //Collision with Checkpoint
    std::queue<Checkpoint*> c = mOwner->GetGame()->GetCheckpoint();
    CollisionComponent* mcc = mOwner->GetComponent<CollisionComponent>();
    Checkpoint* front = nullptr;
    
    if(!mOwner->GetGame()->GetCheckpoint().empty()){
       front = c.front();
    }
    
    if(front){
        CollisionComponent* frontcc = front->GetComponent<CollisionComponent>();
        if(frontcc){
            if(mcc->Intersect(frontcc)){
                Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Checkpoint.wav"), 0);
                reinterpret_cast<Player*>(mOwner)->SetRespawnPos(front->GetPosition());
                front->SetState(ActorState::Destroy);
                //c.pop();
                mOwner->GetGame()->removeCheckpoint();
                //DON'T do c.front()->setState(true). c hasn't been popped yet
                if(!mOwner->GetGame()->GetCheckpoint().empty()){
                    mOwner->GetGame()->GetCheckpoint().front()->setState(true);
                }
            }
        }
    }
    
    for(SecurityCone* sc : mOwner->GetGame()->GetCones()){
        Vector3 camera_to_player = mOwner->GetWorldPosition() - sc->GetWorldPosition();
        Vector3 camera_forward = sc->GetWorldForward();
        float angle = acos(Vector3::Dot(camera_to_player, camera_forward) / (camera_to_player.Length()
        * camera_forward.Length()));
        
        MeshComponent* mc = sc->GetComponent<MeshComponent>();
        float yellow_timer = sc->GetYellowTimer();
        float red_timer = sc->GetRedTimer();
        
        SecurityCamera* parent = reinterpret_cast<SecurityCamera*>(sc->getParent());
        if((angle < CONE_HALF_ANGLE) && camera_to_player.Length() < CONE_HEIGHT)
        {
            /* sound behavior */
            
            int cm_channel = parent->GetCMchannel();
            if(cm_channel != -1){
                Mix_HaltChannel(cm_channel);
            }
            parent->SetAlreadyPlaying(false);
            parent->SetCMchannel(-1);
            if(!parent->GetAlreadyPlaying2()){ Mix_PlayChannel(Mix_GroupAvailable(1),mOwner->GetGame()->GetSound("Assets/Sounds/CameraMotorStop.wav"), 0);
                parent->SetAlreadyPlaying2(true);
            }
            /* sound behavior */
            
            if(yellow_timer < YELLOW_THRESHOLD){
                mc->SetTextureIndex(1);
                yellow_timer += deltaTime;
                sc->SetYellowTimer(yellow_timer);
                if(parent->GetLastColor()){
                    parent->SetLastColor(false);
                    if(parent->GetSDchannel() == -1){
                        parent->SetSDchannel(Mix_PlayChannel(Mix_GroupAvailable(1),
                                                             mOwner->GetGame()->GetSound("Assets/Sounds/SecurityDetected.wav"), 0));
                    }
                    
                }
            }
            else{
                mc->SetTextureIndex(2);
                red_timer += deltaTime;
                sc->SetRedTimer(red_timer);
                parent->SetLastColor(false);
            }
            if(red_timer > RED_THRESHOLD){
                mOwner->SetPosition(reinterpret_cast<Player*>(mOwner)->GetRespawnPos());
                mOwner->SetRotation(0.0f);
                mVelocity = Vector3::Zero;
                mPendingForces = Vector3::Zero;
                ChangeState(MoveState::Falling);
                parent->SetLastColor(true);
            }
        }
        else{
            mc->SetTextureIndex(0);
            yellow_timer = 0.0f;
            sc->SetYellowTimer(yellow_timer);
            red_timer = 0.0f;
            sc->SetRedTimer(red_timer);
            parent->SetLastColor(true);
            if(parent->GetSDchannel() != -1){
                Mix_HaltChannel(parent->GetSDchannel());
                parent->SetSDchannel(-1);
            }
        }
    }
    
    if(mCurrentState == MoveState::Falling)
    {
        Mix_Pause(mRunningSFX);
        UpdateFalling(deltaTime);
    }
    else if(mCurrentState == MoveState::OnGround)
    {
        if(mVelocity.Length() > RUNNING_SOUND_THRESHOLD){
            Mix_Resume(mRunningSFX);
        }
        else{
            Mix_Pause(mRunningSFX);
        }
        UpdateOnGround(deltaTime);
    }
    else if(mCurrentState == MoveState::Jump)
    {
        Mix_Pause(mRunningSFX);
        UpdateJump(deltaTime);
    }
    else if(mCurrentState == MoveState::WallClimb)
    {
        Mix_Resume(mRunningSFX);
        UpdateWallClimb(deltaTime);
    }
    else if (mCurrentState == MoveState::WallRun)
    {
        Mix_Resume(mRunningSFX);
        UpdateWallRun(deltaTime);
    }
}


void PlayerMove::ProcessInput(const Uint8* keyState)
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    int x, y;
    SDL_GetRelativeMouseState(&x, &y);
    float angular_speed_x = float(x) / ANGULAR_DIVIDER * (Math::Pi * AUNGULAR_MULTIPLIER);
    SetAngularSpeed(angular_speed_x);
    
    float angular_speed_y = float(y) / ANGULAR_DIVIDER * (Math::Pi * AUNGULAR_MULTIPLIER);
    CameraComponent* cam = mOwner->GetComponent<CameraComponent>();
    cam->SetPitchSpeed(angular_speed_y);
    
    
    if(state[SDL_SCANCODE_W] && state[SDL_SCANCODE_S]){
    }
    else if(state[SDL_SCANCODE_W]){
        AddForce(mOwner->GetForward() * FORCE_MULTIPLIER);
    }
    else if(state[SDL_SCANCODE_S]){
        AddForce(mOwner->GetForward() * -FORCE_MULTIPLIER);
    }
    
    if(state[SDL_SCANCODE_A] && state[SDL_SCANCODE_D]){
    }
    else if(state[SDL_SCANCODE_A]){
        AddForce(mOwner->GetRight() * -FORCE_MULTIPLIER);
    }
    else if(state[SDL_SCANCODE_D]){
        AddForce(mOwner->GetRight() * FORCE_MULTIPLIER);
    }
    
    //Jumping
    if(state[SDL_SCANCODE_SPACE]){
        curr_frame = true;
        if(last_frame == false && curr_frame == true && mCurrentState == MoveState::OnGround){
            Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Jump.wav"), 0);
            AddForce(mJumpForce);
            ChangeState(MoveState::Jump);
        }
    }else{
        curr_frame = false;
    }
    last_frame = curr_frame;
    
}

void PlayerMove::ChangeState(MoveState mMoveState)
{
    mCurrentState = mMoveState;
}

void PlayerMove::UpdateOnGround(float deltatime)
{
    //MoveComponent::Update(deltatime);
    PhysicsUpdate(deltatime);
    
    bool returned_top = false;
    bool mCanWallClimb = false;
    
    std::vector<Actor*> mBlocks = mOwner->GetGame()->GetBlocks();
    for(Actor* block : mBlocks)
    {
        CollisionComponent::CollSide cs = FixCollision(this->mOwner->GetComponent<CollisionComponent>(),
                                                       block->GetComponent<CollisionComponent>());
        if(cs == CollisionComponent::CollSide::Top){
            returned_top = true;
        }
        else if(cs == CollisionComponent::CollSide::SideMaxY){
            mCanWallClimb = CanWallClimb(cs);
        }
        else if(cs == CollisionComponent::CollSide::SideMaxX){
            mCanWallClimb = CanWallClimb(cs);
        }
        else if(cs == CollisionComponent::CollSide::SideMinY){
            mCanWallClimb = CanWallClimb(cs);
        }
        else if(cs == CollisionComponent::CollSide::SideMinX){
            mCanWallClimb = CanWallClimb(cs);
        }
        
        if(mCanWallClimb){
            mWallClimbTimer = 0.0f;
            ChangeState(MoveState::WallClimb);
            return;
        }
    }
    if(!returned_top){
        ChangeState(MoveState::Falling);
    }
}

void PlayerMove::UpdateJump(float deltatime)
{
    AddForce(mGravity);
    PhysicsUpdate(deltatime);
    bool mCanWallClimb = false;
    bool mCanWallRun = false;
    
    //Loop over all blocks and call FixCollision on each
    //If any blocks return Bottom, it means you hit your head, so set mZSpeed to 0.0f
    std::vector<Actor*> mBlocks = mOwner->GetGame()->GetBlocks();
    for(Actor* block : mBlocks)
    {
        CollisionComponent::CollSide cs = FixCollision(this->mOwner->GetComponent<CollisionComponent>(),
                                                       block->GetComponent<CollisionComponent>());
        if(cs == CollisionComponent::CollSide::Bottom)
        {
            mVelocity.z = 0.0f;
        }
        
        else if(cs == CollisionComponent::CollSide::SideMaxY){
            mCanWallClimb = CanWallClimb(cs);
            mCanWallRun = CanWallRun(cs);
        }
        else if(cs == CollisionComponent::CollSide::SideMaxX){
            mCanWallClimb = CanWallClimb(cs);
            mCanWallRun = CanWallRun(cs);
        }
        else if(cs == CollisionComponent::CollSide::SideMinY){
            mCanWallClimb = CanWallClimb(cs);
            mCanWallRun = CanWallRun(cs);
        }
        else if(cs == CollisionComponent::CollSide::SideMinX){
            mCanWallClimb = CanWallClimb(cs);
            mCanWallRun = CanWallRun(cs);
        }
        
        if(mCanWallClimb){
            mWallClimbTimer = 0.0f;
            ChangeState(MoveState::WallClimb);
            return;
        }
        else if(mCanWallRun){
            mWallRunTimer = 0.0f;
            ChangeState(MoveState::WallRun);
            CurrentSide = cs;
            return;
        }
    }
    
    if(mVelocity.z <= 0.0f){
        ChangeState(MoveState::Falling);
    }
    
}

void PlayerMove::UpdateFalling(float deltatime)
{
    if(mOwner->GetPosition().z < FALLEN_TO_DEATH)
    {
        mOwner->SetPosition(reinterpret_cast<Player*>(mOwner)->GetRespawnPos());
        mOwner->SetRotation(0.0f);
        mVelocity = Vector3::Zero;
        mPendingForces = Vector3::Zero;
        ChangeState(MoveState::Falling);
    }
    
    AddForce(mGravity);
    PhysicsUpdate(deltatime);
    
    std::vector<Actor*> mBlocks = mOwner->GetGame()->GetBlocks();
    for(Actor* block : mBlocks)
    {
        CollisionComponent::CollSide cs = FixCollision(this->mOwner->GetComponent<CollisionComponent>(),
                                                       block->GetComponent<CollisionComponent>());
        if(cs == CollisionComponent::CollSide::Top)
        {
            mVelocity.z = 0.0f;
            ChangeState(MoveState::OnGround);
            Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Land.wav"), 0);
        }
    }
}

void PlayerMove::UpdateWallClimb(float deltatime)
{
    AddForce(mGravity);
    mWallClimbTimer += deltatime;
    if(mWallClimbTimer < MAX_WALL_TIME)
    {
        AddForce(WALL_CLIMB_FORCE);
    }
    
    PhysicsUpdate(deltatime);
    
    bool CollideOnSide = false;
    std::vector<Actor*> mBlocks = mOwner->GetGame()->GetBlocks();
    for(Actor* block : mBlocks)
    {
        CollisionComponent::CollSide cs = FixCollision(this->mOwner->GetComponent<CollisionComponent>(),
                                                       block->GetComponent<CollisionComponent>());
        
        if(cs == CollisionComponent::CollSide::SideMinY ||
           cs == CollisionComponent::CollSide::SideMaxY ||
           cs == CollisionComponent::CollSide::SideMinX ||
           cs == CollisionComponent::CollSide::SideMaxX){
            CollideOnSide = true;
        }
    }
    if(!CollideOnSide ||  mVelocity.z <= 0.0f)
    {
        mVelocity.z = 0.0f;
        ChangeState(MoveState::Falling);
    }
}

void PlayerMove::UpdateWallRun(float deltatime)
{
    AddForce(mGravity);
    mWallRunTimer += deltatime;
    if(mWallRunTimer < MAX_WALL_TIME)
    {
        AddForce(mWallRunForce);
    }
    PhysicsUpdate(deltatime);
    
    std::vector<Actor*> mBlocks = mOwner->GetGame()->GetBlocks();
    for(Actor* block : mBlocks)
    {
        FixCollision(this->mOwner->GetComponent<CollisionComponent>(),block->GetComponent<CollisionComponent>());
        
    }
    if(mVelocity.z <= 0.0f)
    {
        mVelocity.z = 0.0f;
        ChangeState(MoveState::Falling);
    }
}

bool PlayerMove::CanWallClimb(CollisionComponent::CollSide cs)
{
    Vector3 myforward = mOwner->GetForward();
    Vector3 xyVelocity {mVelocity.x, mVelocity.y, 0.0f};
    
    float dot_result = 0.0f;
    float dot_result2 = 0.0f;

    if(cs == CollisionComponent::CollSide::SideMaxY)
    {
        Vector3 maxY {0.0f, 1.0f, 0.0f};
        dot_result = Vector3::Dot(myforward, maxY);
        dot_result2 = Vector3::Dot(xyVelocity, maxY);
    }
    else if(cs == CollisionComponent::CollSide::SideMaxX)
    {
        Vector3 maxX {1.0f, 0.0f, 0.0f};
        dot_result = Vector3::Dot(myforward, maxX);
        dot_result2 = Vector3::Dot(xyVelocity, maxX);
    }
    else if(cs == CollisionComponent::CollSide::SideMinY)
    {
        Vector3 minY {0.0f, -1.0f, 0.0f};
        dot_result = Vector3::Dot(myforward, minY);
        dot_result2 = Vector3::Dot(xyVelocity, minY);
    }
    else if(cs == CollisionComponent::CollSide::SideMinX)
    {
        Vector3 minX {-1.0f, 0.0f, 0.0f};
        dot_result = Vector3::Dot(myforward, minX);
        dot_result2 = Vector3::Dot(xyVelocity, minX);
    }
    
    if(dot_result < -WALLCLIMB_ANGLE && dot_result2 < -WALLCLIMB_ANGLE && xyVelocity.Length() > SPEED){
        return true;
    }
    else{
        return false;
    }
}

bool PlayerMove::CanWallRun(CollisionComponent::CollSide cs)
{
    Vector3 myforward = mOwner->GetForward();
    Vector3 xyVelocity {mVelocity.x, mVelocity.y, 0.0f};
    
    float dot_result = 0.0f;
    float dot_result2 = Vector3::Dot(xyVelocity, myforward);
    
    if(cs == CollisionComponent::CollSide::SideMaxY)
    {
        Vector3 maxY {0.0f, 1.0f, 0.0f};
        dot_result = Vector3::Dot(myforward, maxY);
    }
    else if(cs == CollisionComponent::CollSide::SideMaxX)
    {
        Vector3 maxX {1.0f, 0.0f, 0.0f};
        dot_result = Vector3::Dot(myforward, maxX);
    }
    else if(cs == CollisionComponent::CollSide::SideMinY)
    {
        Vector3 minY {0.0f, -1.0f, 0.0f};
        dot_result = Vector3::Dot(myforward, minY);
    }
    else if(cs == CollisionComponent::CollSide::SideMinX)
    {
        Vector3 minX {-1.0f, 0.0f, 0.0f};
        dot_result = Vector3::Dot(myforward, minX);
    }
    
    if(dot_result < 0.1f && dot_result2 > WALLRUN_ANGLE && xyVelocity.Length() > SPEED &&
       dot_result > -WALLRUN_ANGLE){
        return true;
    }
    else{
        return false;
    }
}

CollisionComponent::CollSide PlayerMove::FixCollision(CollisionComponent* self, CollisionComponent* block)
{
    Vector3 offset;
    CollisionComponent::CollSide mCollSide = self->GetMinOverlap(block, offset);
    if(mCollSide != CollisionComponent::CollSide::None){
        mOwner->SetPosition(mOwner->GetPosition() + offset);
        if(mCollSide == CollisionComponent::CollSide::SideMinY){
            Vector3 SideMinY_NF {0.0f, -WALL_REACTION_FORCE, 0.0f};
            AddForce(SideMinY_NF);
        }
        else if(mCollSide == CollisionComponent::CollSide::SideMaxY){
            Vector3 SideMaxY_NF {0.0f, WALL_REACTION_FORCE, 0.0f};
            AddForce(SideMaxY_NF);
        }
        else if(mCollSide == CollisionComponent::CollSide::SideMinX){
            Vector3 SideMinX_NF {-WALL_REACTION_FORCE, 0.0f, 0.0f};
            AddForce(SideMinX_NF);
        }
        else if(mCollSide == CollisionComponent::CollSide::SideMaxX){
            Vector3 SideMaxX_NF {WALL_REACTION_FORCE, 0.0f, 0.0f};
            AddForce(SideMaxX_NF);
        }
    }

    return mCollSide;
}

void PlayerMove::PhysicsUpdate(float deltaTime)
{
    MoveComponent* mc = mOwner->GetComponent<MoveComponent>();
    mAcceleration = mPendingForces * (1.0f / mMass);
    //Update mVelocity based on mAcceleration and delta time
    mVelocity += mAcceleration * deltaTime;
    
    FixXYVelocity();
    
    //Update the owner’s position based on mVelocity and delta time
    mOwner->SetPosition(mOwner->GetPosition() + mVelocity * deltaTime);
    //Update the owner’s rotation based on angular speed and delta time
    mOwner->SetRotation(mOwner->GetRotation() + mc->GetAngularSpeed() * deltaTime);
    //Set mPendingForces to Vector3::Zero
    mPendingForces = Vector3::Zero;
}

void PlayerMove::AddForce(const Vector3& force)
{
    mPendingForces += force;
}

void PlayerMove::FixXYVelocity()
{
    Vector2 xyVelocity {mVelocity.x, mVelocity.y};
    if(xyVelocity.Length() > MAX_XY_VELOCITY)
    {
        xyVelocity.Normalize();
        xyVelocity *= MAX_XY_VELOCITY;
    }
    if(mCurrentState == MoveState::OnGround || mCurrentState == MoveState::WallClimb)
    {
        if(Math::NearZero(mAcceleration.x))
        {
            xyVelocity.x *= friction;
        }
        if(Math::NearZero(mAcceleration.y))
        {
            xyVelocity.y *= friction;
        }
    }
    
    if((mAcceleration.x > 0.0f && xyVelocity.x < 0.0f) ||
       (mAcceleration.x < 0.0f && xyVelocity.x > 0.0f))
    {
        xyVelocity.x *= friction;
    }
    if((mAcceleration.y > 0.0f && xyVelocity.y < 0.0f) ||
    (mAcceleration.y < 0.0f && xyVelocity.y > 0.0f))
    {
        xyVelocity.y *= friction;
    }
    Vector3 transfer {xyVelocity.x, xyVelocity.y, mVelocity.z};
    mVelocity = transfer;
}

MoveState PlayerMove::getMoveState()
{
    return mCurrentState;
}
