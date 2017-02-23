#include "quad.hpp"
#include <iostream>
size_t Quad::maxDepth_ = 10;
size_t Quad::maxObjs_ = 0;

void Quad::insertObj(Object *obj)
{
    if (!isLeaf) {
        int quadIdx = getObjQuadIdx_(obj);
        if (quadIdx != INVALID_QUAD)
            quads_[quadIdx]->insertObj(obj);
        else
            obj->setQuad(nullptr);
    }
    else if (objs_.size() < maxObjs_ || depth_ >= maxDepth_) {
        objs_.push_back(obj);
        obj->setQuad(this);
    }
    else {

        split();
        objs_.push_back(obj);
        // Insert objects into its sub quads
        for (size_t i=0; i<objs_.size(); i++) {
            Object *o = objs_[i];
            int quadIdx = getObjQuadIdx_(o);
            if (quadIdx != INVALID_QUAD)
                quads_[quadIdx]->insertObj(o);
            else
            {
                std::cout<<"NOt inn \n";
                o->setQuad(nullptr);
            }
        }
        objs_.clear();
        isLeaf = false;
    }
}

void Quad::draw() const
{
    bound_.glDraw();
    for (const auto& q: quads_)
        if (q) q->draw();
}
