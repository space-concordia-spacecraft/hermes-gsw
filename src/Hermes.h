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
    GLFWwindow* m_Window;
    GLFWwindow* m_GlfwContext;
    ImGuiContext* m_Context;
    ImGuiIO m_Io;
    std::vector<GroundStationWindow*> m_Modules;
    ImGuiViewport* m_Viewport;

    /**
     * Creates the GLFW m_Context for Hermes Application
     * @return -1 if m_Context creation failed.
     */
    int SetupGLFW();
    /**
     * Creates the Context for the ImGUI m_Modules in Hermes
     */
    void SetupImGui();

    /**
     * Renders the List of Modules Present in the Module.
     */
    void RenderModules();
    /**
     * Creates the Dockspace, so that the windows can be docked in the Main Application.
     */
    void CreateDockspace();
    /**
     * Creates the Menu bar for the Main Application. This is also the way to interact with the Application.
     */
    void CreateMenuBar();

//    void ProcessInput(GLFWwindow* m_Window) {
//        if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//            glfwSetWindowShouldClose(m_Window, true);
//    }

    /**
     * Polls the Events in the GLFW m_Context and renders the new changes.
     */
    void PollEvents();
    /**
     * Calls PollEvents to Update Windows and swaps GLFW buffers.
     */
    void UpdateWindows();


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
