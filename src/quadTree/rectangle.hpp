#pragma once
#include <glm/glm.hpp>
#include <array>
#include <GLFW/glfw3.h>
#include <imgui.h>
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

    /*! Draw rectangle with ImGui API
     *
     * \param scale 
     * \param translation Translation of the rectangle.
     */
    void ImDraw(float scale=1, glm::vec2 translation = glm::vec2(0.0)) const
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        const ImVec2 p = ImGui::GetCursorScreenPos();
        draw_list->AddRect(
            ImVec2{
            translation,
            }
            ImVec2{
                translation.x + (bounds[1].x - bounds[0].x)*scale,
                translation.y + (bounds[1].y - bounds[0].y)*scale,
                },
            ImColor(ImVec4(1.0, 0.0, 0.0, 1.0)), 1.0f, ~1, 1.0);
    }
};
// Split a rectangle into four. NW, NE, SW, SE
std::array<Rectangle, 4> splitRectangle(const Rectangle& r);
