#include "rectangle.hpp"
std::array<Rectangle, 4> splitRectangle(const Rectangle& r)
{
    glm::vec2 size = (r.bounds[1] - r.bounds[0]) / 2.0f;
    return std::array<Rectangle, 4>{
        Rectangle{glm::vec2(r.bounds[0].x, r.bounds[0].y + size.y),
                  glm::vec2(r.bounds[1].x - size.x, r.bounds[1].y)},
        Rectangle{glm::vec2(r.bounds[0].x + size.x, r.bounds[0].y + size.y),
                  glm::vec2(r.bounds[1].x, r.bounds[1].y)},
        Rectangle{glm::vec2(r.bounds[0].x + size.x, r.bounds[0].y),
                  glm::vec2(r.bounds[1].x, r.bounds[1].y - size.y)},
        Rectangle{glm::vec2(r.bounds[0].x, r.bounds[0].y),
                  glm::vec2(r.bounds[1].x - size.x, r.bounds[1].y - size.y)}};
}
