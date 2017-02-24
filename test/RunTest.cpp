#include <gtest/gtest.h>
#include <random>
#include <array>
#include <glm/glm.hpp>

#include <quad.hpp>
#include <object.hpp>
#include <rectangle.hpp>

const size_t NUM_OBJS = 20000;
const size_t MAX_DEPTH = 10;
const size_t MAX_OBJ_PER_QUAD = 100;
const Rectangle bound{glm::vec2(-1.0), glm::vec2(1.0)};
std::array<Object, NUM_OBJS> objs;
Quad<MAX_DEPTH, MAX_OBJ_PER_QUAD> q(0, bound);

TEST(QuadTree, InesrtionTest)
{   
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> xdist(-0.9, 0.9);
    std::uniform_real_distribution<float> ydist(-0.9, 0.9);
    for (auto &obj: objs)
    {
        obj.setLocation(glm::vec2(xdist(mt), ydist(mt)));
        q.insertObj(&obj);
    }

    EXPECT_TRUE(true);
}

size_t getNumberObjs(Quad<MAX_DEPTH, MAX_OBJ_PER_QUAD> *ptrQ)
{
    if (ptrQ->isLeaf) {
        size_t numObjs = ptrQ->objs().size();
        // ASSERT_TRUE(numObjs <= MAX_OBJ_PER_QUAD);
        // ASSERT_TRUE(ptrQ->getDepth() <= MAX_DEPTH);
        return numObjs;
    }
    else {
        return getNumberObjs(ptrQ->getQuad(0)) +
               getNumberObjs(ptrQ->getQuad(1)) +
               getNumberObjs(ptrQ->getQuad(2)) +
               getNumberObjs(ptrQ->getQuad(3));
    }
}
TEST(QuadTree, validateTree) { 
    // valid number of elements in tree is equal to number of the
    // total objects
    ASSERT_TRUE(getNumberObjs(&q) == NUM_OBJS); }
