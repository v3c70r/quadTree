#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <array>
#include "object.hpp"
#include "rectangle.hpp"
class Quad
{
private:
    static size_t maxDepth_;
    static size_t maxObjs_;
    const static size_t INVALID_QUAD = 4;
    Rectangle bound_;
    size_t numPoints_;
    size_t depth_;
    std::array<Quad*, 4> quads_;
    std::vector<Object*> objs_;
    bool isLeaf;

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
    std::vector<Object*> &objs() {return objs_;}

    Quad(size_t d, Rectangle bound) : bound_(bound), depth_(d), isLeaf(true) {
        for (auto &q: quads_)
            q = nullptr;
    }
    void split()
    {
        if (depth_ < maxDepth_)
        {
            std::array<Rectangle, 4> bounds = splitRectangle(bound_);
            for (size_t i=0; i<quads_.size(); i++)
                quads_[i] = (new Quad(depth_+1, bounds[i]));
        }
    }

    Rectangle getBound() const { return bound_;}

    Quad* getQuad(size_t i) { return quads_[i];}

    ~Quad()
    {
        for (auto &q: quads_)
            if (q) delete q;
    }

    //! Insert an object into quad tree
    void insertObj(Object* obj);

    void draw() const;

};
