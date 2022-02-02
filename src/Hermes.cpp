#include "Hermes.h"

int Hermes::SetupGLFW() {
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_FLOATING, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    m_Window = glfwCreateWindow(1920, 1080, "Hermes", nullptr, nullptr);

    glfwSwapInterval(1);
    glfwShowWindow(m_Window);
    glfwMakeContextCurrent(m_Window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        return -1;
}

void Hermes::SetupImGui(){
    IMGUI_CHECKVERSION();
    m_Context = ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;// Enable Keyboard Controls

    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init("#version 400");
    ImGui::StyleColorsDark();
}

void Hermes::addWindow(GroundStationWindow *window) {
    m_Modules.push_back(window);
}

void Hermes::RenderModules() {
    for(auto window: this->m_Modules){
        window->RenderGUI();
    }
}

void Hermes::CreateDockspace(){
    m_Viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(m_Viewport->Pos);
    ImGui::SetNextWindowSize(m_Viewport->Size);
    ImGui::SetNextWindowViewport(m_Viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
    ImGui::Begin("DockSpace", nullptr, windowFlags);
    ImGui::DockSpace(ImGui::GetID("DockSpace"), { 0.0f, 0.0f }, ImGuiDockNodeFlags_None);
    ImGui::PopStyleVar(3);
}

void Hermes::CreateMenuBar(){
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit", "Alt+F4"))
                glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
            if (ImGui::BeginMenu("Add Window", "Ctrl+F3")) {
                if(ImGui::MenuItem("ADCS", NULL)){
                    this->addWindow((new ImPlotDemo())->GetPtr());
                }
                if(ImGui::MenuItem("Comms", NULL)){
                    this->addWindow((new DemoWindow("Demo Window"))->GetPtr());
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void Hermes::PollEvents(){

    ImGui::SetCurrentContext(m_Context);
    m_Io = ImGui::GetIO();
    int display_w, display_h;
    glfwGetFramebufferSize(m_Window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    m_Io.DisplaySize = ImVec2((float)display_w, (float)display_h);

    if (m_Io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        m_GlfwContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(m_GlfwContext);
    }
    glfwPollEvents();
}

void Hermes::UpdateWindows(){
    PollEvents();
    glfwSwapBuffers(m_Window);
}

int Hermes::run(){
    SetupGLFW();
    SetupImGui();

    while (!glfwWindowShouldClose(m_Window))
    {
        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        CreateDockspace();
        CreateMenuBar();

        ImGui::End();
        RenderModules();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        UpdateWindows();

    }

    ImGui_ImplOpenGL3_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}



