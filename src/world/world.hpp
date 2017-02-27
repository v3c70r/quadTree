#pragma once
#include <vector>
#include <object.hpp>
#include <actor.hpp>
#include <AABBQuadTree.hpp>
#include <chrono>
#include <rectangle.hpp>
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

public:
    World(size_t maxStatic, size_t maxDynamic, Rectangle worldBound):
        MAX_NUM_STATIC_OBJS(maxStatic),
        MAX_NUM_DYNAMIC_OBJS(maxDynamic),
        quadTree_(0, worldBound)
    {
        staticObjs_.reserve(MAX_NUM_STATIC_OBJS);
        actors_.reserve(MAX_NUM_DYNAMIC_OBJS);
    }
    void addStaticObj(Object obj) {staticObjs_.push_back(obj);}
    const std::vector<Object>& getStaticObjs() const { return staticObjs_;}
    void update(std::chrono::milliseconds dt);
};
}
