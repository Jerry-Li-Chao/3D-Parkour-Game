//
//  Block.hpp
//  Lab09
//
//  Created by Apple on 2020/10/22.
//

#ifndef Block_hpp
#define Block_hpp

#include <stdio.h>
#include "Actor.h"

class Block : public Actor
{
public:
    Block(class Game* game, Actor* parent);
    ~Block();
    void SetisMirror(bool m) {isMirror = m;};
    bool GetisMirror() {return isMirror;};
    void SetRotate(bool r) {rotates = r;}
    bool GetRotate() {return rotates;}
    void OnUpdate(float deltaTime) override;
private:
    class MeshComponent* mc;
    class CollisionComponent* mColli;
    bool isMirror = false;
    bool rotates = false;
};

#endif /* Block_hpp */
