#pragma once
#include "quad.hpp"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
class Quad;
class Object
{
private:
    glm::vec2 location_;    //!< the location of the object
    Quad* q_;               //!< the quad the object belong to
public:
    Object(glm::vec2 loc):location_(loc){};

    //virtual void update(float tick) = 0;

    void setLocation(const glm::vec2& p)
    {
        location_ = p;
    }
    glm::vec2 getLocation() const
    {
        return location_;
    }
    void setQuad(Quad* q)
    {
        q_ = q;
    }

    void glDraw() const
    {
        glBegin(GL_POINTS);
            glVertex3f(location_.x, location_.y, 0.0);
        glEnd();
    }
};


