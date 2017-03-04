#pragma once
#include <quadTree/AABB.hpp>
#include <glm/glm.hpp>
#include <vector>
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
    glm::mat4 modelMatrix_;    //!< Model matrix of the object
    std::vector<float> data_;  //!< data of the object, there representation is
                               //!decided by user.
    std::vector<GLuint> index_;

public:
    // Get model matrix
    glm::mat4& modelMatrix() { return modelMatrix_; }
    const glm::mat4& modelMatrix() const{ return modelMatrix_; }
    //! Get data
    std::vector<float>& data() { return data_; }
    const std::vector<float>& data() const { return data_; }
    //! Get Index
    std::vector<GLuint>& index() { return index_;}
    const std::vector<GLuint>& index() const { return index_;}
};
}
