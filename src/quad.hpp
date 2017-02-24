#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <array>
#include "object.hpp"
#include "rectangle.hpp"
template<size_t MAX_DEPTH, size_t MAX_OBJS>
class Quad
{
private:
    const static size_t INVALID_QUAD = 4;
    Rectangle bound_;
    size_t numPoints_;
    size_t depth_;
    std::array<Quad<MAX_DEPTH, MAX_OBJS>*, 4> quads_;
    std::vector<Object*> objs_;

    //! Get the index of subquad the obj belong to
    //! Return 4 if the index is invalid
    size_t getObjQuadIdx_(const Object* obj) const
    {
        glm::vec2 center = (bound_.bounds[1] + bound_.bounds[0]) / 2.0f;
        glm::vec2 sign = obj->getLocation() - center;
        if (sign.x < 0.0f && sign.y > 0.0f)  // first quad
            return 0;
        else if (sign.x > 0.0 && sign.y > 0.0f) 
            return 1;
        else if (sign.x > 0.0 && sign.y < 0.0f)
            return 2;
        else if (sign.x < 0.0 && sign.y < 0.0f)
            return 3;
        else return INVALID_QUAD;
    }

public:
    bool isLeaf;
    std::vector<Object*> &objs() {return objs_;}

    Quad(size_t d, Rectangle bound) : bound_(bound), depth_(d), isLeaf(true) {
        for (auto &q: quads_)
            q = nullptr;
    }
    void split()
    {
        if (depth_ < MAX_DEPTH)
        {
            std::array<Rectangle, 4> bounds = splitRectangle(bound_);
            for (size_t i=0; i<quads_.size(); i++)
                quads_[i] = (new Quad(depth_+1, bounds[i]));
        }
    }

    Rectangle getBound() const { return bound_;}

    size_t getDepth() const { return depth_;}
    Quad<MAX_DEPTH, MAX_OBJS>* getQuad(size_t i) { return quads_[i];}

    ~Quad()
    {
        for (auto &q: quads_)
            if (q) delete q;
    }

    //! Insert an object into quad tree
    void insertObj(Object *obj)
    {
        if (!isLeaf) {
            int quadIdx = getObjQuadIdx_(obj);
            if (quadIdx != INVALID_QUAD) quads_[quadIdx]->insertObj(obj);
        }
        else if (objs_.size() < MAX_OBJS || depth_ >= MAX_DEPTH) {
            objs_.push_back(obj);
        }
        else {
            split();
            objs_.push_back(obj);
            // Insert objects into its sub quads
            for (size_t i = 0; i < objs_.size(); i++) {
                Object *o = objs_[i];
                int quadIdx = getObjQuadIdx_(o);
                if (quadIdx != INVALID_QUAD) quads_[quadIdx]->insertObj(o);
            }
            objs_.clear();
            isLeaf = false;
        }
    }

    void draw() const
    {
        bound_.glDraw();
        for (const auto &q : quads_)
            if (q) q->draw();
    }
};
