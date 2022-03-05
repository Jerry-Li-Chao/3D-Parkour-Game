#include "CollisionComponent.h"
#include "Actor.h"
#include <algorithm>

CollisionComponent::CollisionComponent(class Actor* owner)
:Component(owner)
,mWidth(0.0f)
,mHeight(0.0f)
,mDepth(0.0f)
{
	
}

CollisionComponent::~CollisionComponent()
{
	
}

bool CollisionComponent::Intersect(const CollisionComponent* other)
{
	Vector3 aMin = GetMin();
	Vector3 aMax = GetMax();
	Vector3 bMin = other->GetMin();
	Vector3 bMax = other->GetMax();

	bool no = aMax.x < bMin.x ||
		aMax.y < bMin.y ||
		aMax.z < bMin.z ||
		bMax.x < aMin.x ||
		bMax.y < aMin.y ||
		bMax.z < aMin.z;

	return !no;
}

Vector3 CollisionComponent::GetMin() const
{
	Vector3 v = mOwner->GetPosition();
	v.x -= mDepth * mOwner->GetScale() / 2.0f;
	v.y -= mWidth * mOwner->GetScale() / 2.0f;
	v.z -= mHeight * mOwner->GetScale() / 2.0f;
	return v;
}

Vector3 CollisionComponent::GetMax() const
{
	Vector3 v = mOwner->GetPosition();
	v.x += mDepth * mOwner->GetScale() / 2.0f;
	v.y += mWidth * mOwner->GetScale() / 2.0f;
	v.z += mHeight * mOwner->GetScale() / 2.0f;
	return v;
}

const Vector3& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollisionComponent::CollSide CollisionComponent::GetMinOverlap(
	const CollisionComponent* other, Vector3& offset)
{
    if(Intersect(other)){
        float MinZDiff = other->GetMin().z - this->GetMax().z;
        float MaxZDiff = other->GetMax().z - this->GetMin().z;
        float otherMinYDiff = other->GetMin().y - this->GetMax().y; //unit normal vector: <0, -1, 0>
        float otherMinXDiff = other->GetMin().x - this->GetMax().x; //unit normal vector: <-1, 0, 0>
        float otherMaxYDiff = other->GetMax().y - this->GetMin().y; //unit normal vector: <0, 1, 0>
        float otherMaxXDiff = other->GetMax().x - this->GetMin().x; //unit normal vector: <1, 0, 0>
        
        float abs_otherMinZDiff = std::abs(MinZDiff);
        float abs_otherMaxZDiff = std::abs(MaxZDiff);
        float abs_otherMinYDiff = std::abs(otherMinYDiff);
        float abs_otherMinXDiff = std::abs(otherMinXDiff);
        float abs_otherMaxYDiff = std::abs(otherMaxYDiff);
        float abs_otherMaxXDiff = std::abs(otherMaxXDiff);
        float min_val = std::min({abs_otherMinYDiff, abs_otherMinXDiff,
            abs_otherMaxYDiff, abs_otherMaxXDiff, abs_otherMinZDiff, abs_otherMaxZDiff});
        //collide with top side
        if(min_val == abs_otherMaxZDiff){
            Vector3 shift1 {0.0f, 0.0f, MaxZDiff};
            offset = shift1;
            return CollSide::Top;
        }
        //collide with bottom side
        else if(min_val == abs_otherMinZDiff){
            Vector3 shift2 {0.0f, 0.0f, MinZDiff};
            offset = shift2;
            return CollSide::Bottom;
        }
        else{
            if(min_val == abs_otherMinYDiff){
                Vector3 shift3 {0.0f, otherMinYDiff, 0.0f};
                offset = shift3;
                return CollSide::SideMinY;
            }
            else if(min_val == abs_otherMaxYDiff){
                Vector3 shift4 {0.0f, otherMaxYDiff, 0.0f};
                offset = shift4;
                return CollSide::SideMaxY;
            }
            else if(min_val == abs_otherMinXDiff){
                Vector3 shift5 {otherMinXDiff, 0.0f, 0.0f};
                offset = shift5;
                return CollSide::SideMinX;
            }
            else if(min_val == abs_otherMaxXDiff){
                Vector3 shift6 {otherMaxXDiff, 0.0f, 0.0f};
                offset = shift6;
                return CollSide::SideMaxX;
            }
            else{return CollSide::None;}
        }
        
    }
    //return CollSide::None;
	offset = Vector3::Zero;
	return CollSide::None;
}

