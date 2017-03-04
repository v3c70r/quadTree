#pragma once
//#include <rectangle.hpp>
#include "AABB.hpp"

#include <array>
#include <glm/glm.hpp>
#include <vector>
namespace QT {
/*! Quad tree to store AABB
 * Quad tree in which the objects are AABBs. It would be more
 * useful in collision detection than point based QuadTree.
 * Can subsititute point based quadtree when the bounding box
 * is a point.
*/
template <size_t MAX_DEPTH, size_t MAX_OBJS>
class AABBQuad {
protected:
    const static size_t INVALID_QUAD = 4;
    Rectangle bound_;
    size_t depth_;
    std::array<AABBQuad<MAX_DEPTH, MAX_OBJS> *, 4> quads_;
    std::vector<AABB *> objs_;

public:
    bool isLeaf;
    std::vector<AABB *> &getObjs() { return objs_; }
    AABBQuad(size_t d, Rectangle bound) : bound_(bound), depth_(d), isLeaf(true)
    {
        for (auto &q : quads_) q = nullptr;
    }
    void split()
    {
        if (depth_ < MAX_DEPTH) {
            std::array<Rectangle, 4> bounds = splitRectangle(bound_);
            for (size_t i = 0; i < quads_.size(); i++)
                quads_[i] = (new AABBQuad(depth_ + 1, bounds[i]));
        }
    }
    Rectangle getBound() const { return bound_; }
    ~AABBQuad()
    {
        for (auto &q : quads_)
            if (q) delete q;
    }

    //! Insert an object into quad tree
    void insertObj(AABB *obj)
    {
        if (!bound_.overlaps(obj->getBoundingBox())) return;
        if (!isLeaf) {
            for (auto q : quads_) q->insertObj(obj);
        }
        else if (objs_.size() < MAX_OBJS || depth_ >= MAX_DEPTH) {
            objs_.push_back(obj);
        }
        else {
            split();
            objs_.push_back(obj);
            // Insert objects into its sub quads

            for (const auto &obj : objs_) {
                for (auto q : quads_) q->insertObj(obj);
            }
            objs_.clear();
            isLeaf = false;
        }
    }
    // Draw Quadtree with ImGui
    void ImDraw(float scale, const Rectangle &boundary) const
    {
        bound_.ImDraw(scale, boundary);
        for (const auto &q : quads_)
            if (q) q->ImDraw(scale, boundary);
    }
};
}
