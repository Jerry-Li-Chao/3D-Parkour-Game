//
//  LaserComponent.cpp
//  Lab10
//
//  Created by Apple on 2020/11/3.
//

#include "LaserComponent.hpp"
#include "Shader.h"
#include "Mesh.h"
#include "VertexArray.h"
#include "Actor.h"
#include "Texture.h"
#include "Game.h"
#include "Renderer.h"
#include "SegmentCast.h"
#include "Player.hpp"
#include "Block.hpp"
#include "PlayerMove.hpp"

LaserComponent::LaserComponent(Actor* owner, bool usesAlpha)
:MeshComponent(owner, usesAlpha)
{
    SetMesh(mOwner->GetGame()->GetRenderer()->GetMesh("Assets/Laser.gpmesh"));
}

void LaserComponent::Update(float deltaTime)
{
    mLineSegment_vec.clear();
    mLineSegment.mStart = mOwner->GetWorldPosition();
    mLineSegment.mEnd = mOwner->GetWorldPosition() + mOwner->GetWorldForward() * FORWARD_DIST;
    
    CastInfo outInfo;
    //If segment does hit the player
    bool result = SegmentCast(mOwner->GetGame()->GetPlayer(), mLineSegment, outInfo);
    if(result)
    {
        Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Laser.wav"), 0);
        mLineSegment.mEnd = outInfo.mPoint;
        mLineSegment_vec.emplace_back(mLineSegment);
        
        Player* mPlayer = mOwner->GetGame()->GetPlayer();
        mPlayer->SetPosition(mPlayer->GetRespawnPos());
        mPlayer->SetRotation(0.0f);
        PlayerMove* pm = mPlayer->getPlayerMove();
        pm->SetVelocity(Vector3::Zero);
        pm->SetPendingForce(Vector3::Zero);
        pm->ChangeState(MoveState::Falling);
        //pm->SetlastcolorisWhite(true);
    }
    //If segment does not hit the player
    else{
        if(SegmentCast(mOwner->GetGame()->GetBlocks(), mLineSegment, outInfo, mOwner->getParent()))
        {
            mLineSegment.mEnd = outInfo.mPoint;
            mLineSegment_vec.emplace_back(mLineSegment);
            Block* b = reinterpret_cast <Block*>(outInfo.mActor);
            bool first = true;
            Vector3 r;
            while(b->GetisMirror()) {
                
                if(first){
                    r = Vector3::Reflect(mOwner->GetQuatForward(), outInfo.mNormal);
                    first = false;
                }
                else{
                    r = Vector3::Reflect(r, outInfo.mNormal);
                }
                r.Normalize();
                Vector3 end = outInfo.mPoint + r * FORWARD_DIST;
                LineSegment ls = LineSegment(outInfo.mPoint, end);
                mLineSegment_vec.emplace_back(ls);
                bool result2 = SegmentCast(mOwner->GetGame()->GetBlocks(), ls, outInfo,
                                           reinterpret_cast <Actor*>(b));
                if(result2) {b = reinterpret_cast <Block*>(outInfo.mActor);}
                else{
                    break;
                }
            }
        }
        else{
            mLineSegment_vec.emplace_back(mLineSegment);
        }
    }
    
    
    
}

void LaserComponent::Draw(class Shader* shader)
{
    if (mMesh)
    {
        // Set the active texture
        Texture* t = mMesh->GetTexture(mTextureIndex);
        if (t)
        {
            t->SetActive();
        }
        // Set the mesh's vertex array as active
        VertexArray* va = mMesh->GetVertexArray();
        va->SetActive();
        
        for(auto ls : mLineSegment_vec)
        {
            // Set the world transform
            shader->SetMatrixUniform("uWorldTransform",
                ObjectToWorld(ls));
            
            // Draw
            glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
        }
    }
}

Matrix4 LaserComponent::ObjectToWorld(LineSegment ls)
{
    Matrix4 scale = Matrix4::CreateScale(ls.Length(), 1.0f, 1.0f);
    //Matrix4 rotation = Matrix4::CreateRotationZ(mOwner->GetRotation());
    Matrix4 rotation;
    
    Vector3 ls_facing = ls.mEnd - ls.mStart;
    ls_facing.Normalize();
    float dot_product = Vector3::Dot(Vector3::UnitX, ls_facing);
    if(dot_product == 1){
        rotation = Matrix4::CreateFromQuaternion(Quaternion::Identity);
    }
    else if(dot_product == -1){
        rotation = Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitZ, Math::Pi));
    }
    else{
        float angle = acos(dot_product);
        Vector3 axis = Vector3::Cross(Vector3::UnitX, ls_facing);
        axis.Normalize();
        rotation = Matrix4::CreateFromQuaternion(Quaternion(axis, angle));
    }
    
    
    Matrix4 translation = Matrix4::CreateTranslation(ls.PointOnSegment(0.5f));
    
    return scale * rotation * translation;
}
