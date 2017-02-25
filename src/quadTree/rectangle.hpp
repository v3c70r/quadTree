#pragma once
#include <glm/glm.hpp>
#include <array>
#include <GLFW/glfw3.h>
// Test quadtree
struct Rectangle
{
    std::array<glm::vec2, 2> bounds;    //!< bound: p[0] is bottom left, p[1] is top right
    bool overlaps(const Rectangle &r);

    //! Test if point in the rectangle
    bool contains(const glm::vec2 &p)
    {
        return p.x > bounds[0].x && p.x < bounds[1].x && p.y > bounds[0].y &&
               p.y < bounds[1].y;
    }
    void glDraw() const
    {
        glBegin(GL_LINE_LOOP);
            glVertex3f(bounds[0].x, bounds[0].y, 0.0f);
            glVertex3f(bounds[1].x, bounds[0].y, 0.0f);
            glVertex3f(bounds[1].x, bounds[1].y, 0.0f);
            glVertex3f(bounds[0].x, bounds[1].y, 0.0f);
        glEnd();
    }
};
// Split a rectangle into four. NW, NE, SW, SE
std::array<Rectangle, 4> splitRectangle(const Rectangle& r);
