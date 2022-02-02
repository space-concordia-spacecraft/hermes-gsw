//
// Created by Akhilesh Warty on 01/02/2022.
//

#ifndef GSW_HERMES_H
#define GSW_HERMES_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <ImGuizmo.h>
#include <implot.h>
#include "Modules.h"

class Hermes{
private:
    int windowFlags =ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking |ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    GLFWwindow* window;
    GLFWwindow* glfwContext;
    ImGuiContext* context;
    ImGuiIO io;
    std::vector<GroundStationWindow*> modules;
    std::vector<bool> visibility;
    ImGuiViewport* viewport;

    /**
     * Creates the GLFW context for Hermes Application
     * @return -1 if context creation failed.
     */
    int setupGLFW();
    /**
     * Creates the Context for the ImGUI modules in Hermes
     */
    void setupImGui();

    /**
     * Renders the List of Modules Present in the Module.
     */
    void renderModules();
    /**
     * Creates the Dockspace, so that the windows can be docked in the Main Application.
     */
    void createDockspace();
    /**
     * Creates the Menu bar for the Main Application. This is also the way to interact with the Application.
     */
    void createMenuBar();

//    void ProcessInput(GLFWwindow* window) {
//        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//            glfwSetWindowShouldClose(window, true);
//    }

    /**
     * Polls the Events in the GLFW context and renders the new changes.
     */
    void pollEvents();
    /**
     * Calls pollEvents to Update Windows and swaps GLFW buffers.
     */
    void updateWindows();


public:

    /**
    * Adds the Module to Hermes Main Window.
    * @param window
    */
    void addWindow(GroundStationWindow* window);

    /**
     * Creates Hermes and Runs the Entire Application
     * @return
     */
    int run();



};
#endif //GSW_HERMES_H
