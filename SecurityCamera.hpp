//
//  SecurityCamera.hpp
//  Lab11
//
//  Created by Apple on 2020/11/12.
//

#ifndef SecurityCamera_hpp
#define SecurityCamera_hpp

#include <stdio.h>
#include "Actor.h"
#include "Math.h"

class SecurityCamera : public Actor
{
public:
    SecurityCamera(class Game* game, Actor* parent);
    ~SecurityCamera();
    void setStartQ(Quaternion startQ_) {startQ = startQ_;}
    void setEndQ(Quaternion EndQ_) {endQ = EndQ_;}
    void setinterpTime(float i) {interpTime = i;}
    void setpauseTime(float p) {pauseTime = p;}
    void OnUpdate(float deltaTime) override;
    
    int GetCMchannel() {return cm_channel;}
    void SetCMchannel(int cm) {cm_channel = cm;}
    int GetSDchannel() {return sd_channel;}
    void SetSDchannel(int sd) {sd_channel = sd;}
    bool GetLastColor() {return last_color_is_white;}
    void SetLastColor(bool lc) {last_color_is_white = lc;}
    
    void SetAlreadyPlaying(bool ap) {already_playing = ap;}
    
    void SetAlreadyPlaying2(bool ap2) {already_playing2 = ap2;}
    bool GetAlreadyPlaying2() {return already_playing2;}
private:
    class MeshComponent* mc;
    Quaternion startQ;
    Quaternion endQ;
    float interpTime;
    float pauseTime;
    float timer = 0.0f;
    float pause_timer = 0.0f;
    bool startToend = true;
    bool endTostart = false;
    bool pause = false;
    bool already_playing = false;
    bool already_playing2 = false;
    int cm_channel = -1;
    int sd_channel = -1;
    bool last_color_is_white = true;
    const float LERP_MAX = 1500.0f;
    const float LERP_MIN = 500.0f;
};


#endif /* SecurityCamera_hpp */
