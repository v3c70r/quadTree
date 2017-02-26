#pragma once
#include <AABB.hpp>
#include <glm/glm.hpp>
namespace GAME {
/*! Base class for object in the world
 *
 * * 2D and 3D coordinates are in the same world coordinates. x,y components of
 * 3D coordiantes can be used to represent the location of the points on the
 * map(QuadTree)
 * * Each object has no hierarchical structure.
 */
class Object : public QT::AABB {
protected:
    glm::vec4 modelMatrix_;
};
}
