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

    //! Helper functions
    float& minX() { return bounds[0].x;}
    float& minY() { return bounds[0].y;}
    float& maxX() { return bounds[1].x;}
    float& maxY() { return bounds[1].y;}

    //! Test if point in the rectangle
    bool contains(const glm::vec2 &p)
    {
        return p.x > bounds[0].x && p.x < bounds[1].x && p.y > bounds[0].y &&
               p.y < bounds[1].y;
    }

    /*! Draw rectangle with ImGui API
     *
     * \param scale 
     * \param The bounding box for the box to draw within.
     */
    void ImDraw(float scale, const Rectangle &boudingBox,
                ImColor color = ImVec4(1.0, 1.0, 1.0, 1.0),
                bool filled = false) const
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        const ImVec2 p = ImGui::GetCursorScreenPos();
        float xOffset = (bounds[0].x - boudingBox.bounds[0].x) * scale;
        float yOffset = (boudingBox.bounds[1].y - bounds[1].y) * scale;
        ImVec2 startPoint{p.x + xOffset, p.y + yOffset};

        if (filled)
            draw_list->AddRectFilled(
                startPoint,
                ImVec2{startPoint.x + (bounds[1].x - bounds[0].x) * scale,
                       startPoint.y + (bounds[1].y - bounds[0].y) * scale},
                color);

        else
            draw_list->AddRect(
                startPoint,
                ImVec2{startPoint.x + (bounds[1].x - bounds[0].x) * scale,
                       startPoint.y + (bounds[1].y - bounds[0].y) * scale},
                color, 1.0f, ~1, 1.0);
    }
};
// Split a rectangle into four. NW, NE, SW, SE
std::array<Rectangle, 4> splitRectangle(const Rectangle& r);
