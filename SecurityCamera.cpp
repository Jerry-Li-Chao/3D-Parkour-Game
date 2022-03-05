//
//  SecurityCamera.cpp
//  Lab11
//
//  Created by Apple on 2020/11/12.
//

#include "SecurityCamera.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "SecurityCone.hpp"
#include <iostream>
#include "Player.hpp"

SecurityCamera::SecurityCamera(Game* game, Actor* parent)
:Actor(game, parent)
{
    mc = new MeshComponent(this);
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Camera.gpmesh"));
    new SecurityCone(game, this);
}

SecurityCamera::~SecurityCamera()
{
    if(cm_channel != -1){
        Mix_HaltChannel(cm_channel);
    }
}

void SecurityCamera::OnUpdate(float deltaTime)
{
    Player* mPlayer = mGame->GetPlayer();
    Vector3 camera_to_player = mPlayer->GetWorldPosition() - GetWorldPosition();
    float dist = camera_to_player.Length();

    if(cm_channel != -1){
        if(dist > LERP_MAX){
            Mix_Volume(cm_channel, 0);
        }
        else if(dist > LERP_MIN && dist <= LERP_MAX){
            float dist2 = dist - LERP_MIN;
            float f = dist2 / 1000.0f;
            
            int temp = int(Math::Lerp(0.0f, 128.0f, 1-f));
            Mix_Volume(cm_channel, temp);
        }
        else if(dist > 0.0f && dist <= LERP_MIN){
            Mix_Volume(cm_channel, 128);
        }
    }
    
    size_t index = getChildren().at(0)->GetComponent<MeshComponent>()->getTextureIndex();
    //std::cout << std::boolalpha << "already_playing: " << already_playing << std::endl;
    if(index == 0){
        setQuaternion(Quaternion::Slerp(startQ, endQ, timer/interpTime));
        if(startToend){
            //std::cout << "startToend" << std::endl;
            already_playing2 = false;
            if(!already_playing){
                cm_channel = Mix_PlayChannel(Mix_GroupAvailable(1),mGame->GetSound("Assets/Sounds/CameraMotor.wav"), 0);
                if(dist > LERP_MAX){
                    Mix_Volume(cm_channel, 0);
                }
                else if(dist > LERP_MIN && dist <= LERP_MAX){
                    float dist2 = dist - LERP_MIN;
                    float f = dist2 / 1000.0f;
                    int temp = int(Math::Lerp(0.0f, 128.0f, 1-f));
                    Mix_Volume(cm_channel, temp);
                }
                else if(dist > 0.0f && dist <= LERP_MIN){
                    Mix_Volume(cm_channel, 128);
                }
                
                already_playing = true;
            }
            timer += deltaTime;
            if(timer > interpTime){
                startToend = false;
                pause = true;
            }
        }
        
        if(pause){
            //std::cout << "pause" << std::endl;
            if(cm_channel != -1){
                Mix_HaltChannel(cm_channel);
            }
            already_playing = false;
            cm_channel = -1;
            if(!already_playing2){
                int temp2 = Mix_PlayChannel(Mix_GroupAvailable(1),mGame->GetSound("Assets/Sounds/CameraMotorStop.wav"), 0);
                if(dist > LERP_MAX){
                    Mix_Volume(temp2, 0);
                }
                else if(dist > LERP_MIN && dist <= LERP_MAX){
                    float dist2 = dist - LERP_MIN;
                    float f = dist2 / 1000.0f;
                    int temp = int(Math::Lerp(0.0f, 128.0f, 1-f));
                    Mix_Volume(temp2, temp);
                }
                else if(dist > 0.0f && dist <= LERP_MIN){
                    Mix_Volume(temp2, 128);
                }
                already_playing2 = true;
            }
            
            pause_timer += deltaTime;
            if(pause_timer > pauseTime){
                pause_timer = 0.0f;
                pause = false;
                if(timer > interpTime){
                    endTostart = true;
                }
                if(timer < 0.0f){
                    startToend = true;
                }
            }
        }
        
        if(endTostart){
            //std::cout << "endTostart" << std::endl;
            already_playing2 = false;
            if(!already_playing){
                cm_channel = Mix_PlayChannel(Mix_GroupAvailable(1),mGame->GetSound("Assets/Sounds/CameraMotor.wav"), 0);
                if(dist > LERP_MAX){
                    Mix_Volume(cm_channel, 0);
                }
                else if(dist > LERP_MIN && dist <= LERP_MAX){
                    float dist2 = dist - LERP_MIN;
                    float f = dist2 / 1000.0f;
                    int temp = int(Math::Lerp(0.0f, 128.0f, 1-f));
                    Mix_Volume(cm_channel, temp);
                }
                else if(dist > 0.0f && dist <= LERP_MIN){
                    Mix_Volume(cm_channel, 128);
                }
                already_playing = true;
            }
            timer -= deltaTime;
            if(timer < 0.0f){
                endTostart = false;
                pause = true;
            }
        }
    }
    //stop rotating behavior
    else{
        already_playing = false;
    }
}
