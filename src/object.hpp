#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
class Object
{
private:
    glm::vec2 location_;    //!< the location of the object
public:
    Object():location_(glm::vec2(0.0)){}
    Object(glm::vec2 loc):location_(loc){}

    //virtual void update(float tick) = 0;

    void setLocation(const glm::vec2& p)
    {
        location_ = p;
    }
    glm::vec2 getLocation() const
    {
        return location_;
    }

    // Draw function
    void glDraw() const
    {
        glBegin(GL_POINTS);
            glVertex3f(location_.x, location_.y, 0.0);
        glEnd();
    }
};


