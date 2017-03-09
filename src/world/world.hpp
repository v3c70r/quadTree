#pragma once
#include <vector>
#include "object.hpp"
#include "actor.hpp"
#include <quadTree/AABBQuadTree.hpp>
#include <chrono>
#include <quadTree/rectangle.hpp>
#include <world/FPScamera.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace GAME
{
/*! Game world!
 *
 * * Pack static objects together to form one huge static buffer
 */
class World
{
protected:
    const size_t MAX_NUM_STATIC_OBJS;   //!< Maximum number of static objects
    const size_t MAX_NUM_DYNAMIC_OBJS;  //!< Maximum number of dynamic objects
    std::vector<Object> staticObjs_;    //!< Static objects
    std::vector<Actor> actors_;         //!< Dynamic objects
    QT::AABBQuad<10, 100> quadTree_;
    FPScamera camera_;

public:
    World(size_t maxStatic, size_t maxDynamic, Rectangle worldBound):
        MAX_NUM_STATIC_OBJS(maxStatic),
        MAX_NUM_DYNAMIC_OBJS(maxDynamic),
        quadTree_(0, worldBound)
    {
        staticObjs_.reserve(MAX_NUM_STATIC_OBJS);
        actors_.reserve(MAX_NUM_DYNAMIC_OBJS);

        camera_.cameraMatrix() = glm::lookAt(glm::vec3(0.0, 10.0, -20.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
        camera_.projectionMatrix() = glm::perspective(1.0f, 1.0f, 0.1f, 100.0f);
    }
    const FPScamera& camera() const { return camera_; }
    FPScamera& camera() { return camera_; }
    void addStaticObj(Object obj) {staticObjs_.push_back(obj);}
    const std::vector<Object>& getStaticObjs() const { return staticObjs_;}
    void update(std::chrono::milliseconds dt);
};
}
