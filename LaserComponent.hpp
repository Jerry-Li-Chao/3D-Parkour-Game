//
//  LaserComponent.hpp
//  Lab10
//
//  Created by Apple on 2020/11/3.
//

#ifndef LaserComponent_hpp
#define LaserComponent_hpp

#include <stdio.h>
#include "MeshComponent.h"
#include "SegmentCast.h"
#include "Math.h"

class LaserComponent : public MeshComponent
{
public:
    LaserComponent(class Actor* owner, bool usesAlpha = true);
    void Update(float deltaTime) override;
    void Draw(class Shader* shader) override;
private:
    Matrix4 ObjectToWorld(LineSegment ls);
    struct LineSegment mLineSegment;
    std::vector<struct LineSegment> mLineSegment_vec;
    const float FORWARD_DIST = 500.0f;
};

#endif /* LaserComponent_hpp */
