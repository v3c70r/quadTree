#pragma once
#include "rectangle.hpp"
namespace QT {

//! Axis Alinged Bounding Box
class AABB {
protected:
    Rectangle boundingBox_;  //!< Bouding box
public:
    AABB(){}
    AABB(Rectangle r) : boundingBox_(r) {}
    Rectangle getBoundingBox() const { return boundingBox_; }
    void setBoundingBox(Rectangle r) { boundingBox_ = r; }
};
}
