#include "Hermes.h"

int Hermes::setupGLFW() {
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_FLOATING, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(1920, 1080, "Hermes", nullptr, nullptr);

    glfwSwapInterval(1);
    glfwShowWindow(window);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        return -1;
}

void Hermes::setupImGui(){
    IMGUI_CHECKVERSION();
    context = ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;// Enable Keyboard Controls

    ImGui_ImplGlfw_InitForOpenGL(window,true);
    ImGui_ImplOpenGL3_Init("#version 400");
    ImGui::StyleColorsDark();
}

void Hermes::addWindow(GroundStationWindow *window) {
    modules.push_back(window);
}

void Hermes::renderModules() {
    for(auto window: this->modules){
        window->renderGUI();
    }
}

void Hermes::createDockspace(){
    viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
    ImGui::Begin("DockSpace", nullptr, windowFlags);
    ImGui::DockSpace(ImGui::GetID("DockSpace"), { 0.0f, 0.0f }, ImGuiDockNodeFlags_None);
    ImGui::PopStyleVar(3);
}

void Hermes::createMenuBar(){
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit", "Alt+F4"))
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            if (ImGui::BeginMenu("Add Window", "Ctrl+F3")) {
                if(ImGui::MenuItem("ADCS", NULL)){
                    this->addWindow((new ImPlotDemo())->getPtr());
                }
                if(ImGui::MenuItem("Comms", NULL)){
                    this->addWindow( (new DemoWindow("Demo Window"))->getPtr());
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void Hermes::pollEvents(){

    ImGui::SetCurrentContext(context);
    io = ImGui::GetIO();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    io.DisplaySize = ImVec2((float)display_w, (float)display_h);

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        glfwContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(glfwContext);
    }
    glfwPollEvents();
}

void Hermes::updateWindows(){
    pollEvents();
    glfwSwapBuffers(window);
}

int Hermes::run(){
    setupGLFW();
    setupImGui();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        createDockspace();
        createMenuBar();

        ImGui::End();
        renderModules();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        updateWindows();

    }

    ImGui_ImplOpenGL3_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}



