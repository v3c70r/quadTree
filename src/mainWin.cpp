#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include <iostream>
#include <array>
#include <random>

#include <rectangle.hpp>
#include <quad.hpp>
#include <object.hpp>

#include <imgui.h>
#include <examples/opengl3_example/imgui_impl_glfw_gl3.h>

#include <AABB.hpp>
#include <AABBQuadTree.hpp>

// //////////////////////
static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action,
                         int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

std::vector<Object> points;
Quad<5, 10> q(0, Rectangle{glm::vec2(-0.9), glm::vec2(0.9)});

std::vector<QT::AABB> aabbPoints;
QT::AABBQuad<5, 2> aabbQ(0, Rectangle{glm::vec2(-0.9), glm::vec2(0.9)});

static bool isDragging = false;
static bool insertPos = false;

static void mouse_button_callback(GLFWwindow* window, int button, int action,
                                  int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if( action == GLFW_PRESS)
        {
            if (!isDragging)
            {
                isDragging = true;
                insertPos = true;
            }
        }
        else
        {
            isDragging = false;
        }
    }
}


static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (isDragging)
    {
        int w, h;
        glfwGetWindowSize(window, &w, &h);
        if (insertPos)
        {
            points.push_back(glm::vec2(2.0*(xpos/w) - 1.0,1.0-(ypos/h)*2.0));
            std::cout<<&(points.back())<<std::endl;
            q.insertObj(&(points.back()));
            insertPos = false;
        }
        else
        {
            //points.back().setLocation(glm::vec2(xpos/w, ypos/h));
        }
    }
}

int main(void)
{

    points.reserve(2000);
    GLFWwindow* window;
    std::random_device rd;
    std::mt19937 mt(rd());
    std::normal_distribution<float> dist(0.0, 0.2);
    for (size_t i=0; i<1000; i++)
    {
        points.push_back(Object{glm::vec2(dist(mt), dist(mt))});
        q.insertObj(&(points.back()));
    }

    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);


    glewInit();
    // Setup ImGui binding
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
    

    //aabbPoints.push_back(QT::AABB(Rectangle{glm::vec2(-0.2), glm::vec2(0.3)}));
    //aabbPoints.push_back(QT::AABB(Rectangle{glm::vec2(-0.2), glm::vec2(0.3)}));
    //aabbPoints.push_back(QT::AABB(Rectangle{glm::vec2(-0.1), glm::vec2(0.3)}));
    for (size_t i=0; i<aabbPoints.size(); i++)
        aabbQ.insertObj(&(aabbPoints[i]));


    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();
        ImGui::Text("Hello, world!");

        // Experiment on draw rectangles!!
        // ///////
        float sz = 50.0;
        //draw_list->AddRect(ImVec2(p.x, p.y), ImVec2(p.x+sz, p.y+sz), ImColor(ImVec4(1.0, 0.0, 0.0, 1.0)), 1.0f, ~1, 1.0);
        Rectangle r1{glm::vec2(10.0, 10.0),
                    glm::vec2(50.0, 30.0)};
        Rectangle r2{glm::vec2(0.0, 0.0),
                    glm::vec2(50.0, 30.0)};
        ImGui::Begin("Rectangles");
        r1.ImDraw(1);
        r2.ImDraw(1);
        ImGui::End();

        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float)height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1.0, 1.0, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glPointSize(5.0);
        for (const auto& pt : points) {
            pt.glDraw();
        }
        glColor3f(1.0, 0.0, 0.0);
        for (const auto& aabb: aabbPoints)
        {
            aabb.getBoundingBox().glDraw();
        }
        glColor3f(0.0, 1.0, 0.0);
        q.draw();
        ImGui::Render();
        glfwSwapBuffers(window);
    }
    ImGui_ImplGlfwGL3_Shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
