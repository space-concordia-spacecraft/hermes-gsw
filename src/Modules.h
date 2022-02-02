#ifndef GSW_MODULES_H
#define GSW_MODULES_H

class GroundStationWindow{
public:
    virtual void RenderGUI()=0;
    virtual GroundStationWindow* GetPtr()=0;
};

class DemoWindow: public GroundStationWindow{
    const string m_WindowName;
public:
    DemoWindow(string name): m_WindowName(name){
    }
    void RenderGUI() override{
        ImGui::Begin(m_WindowName.c_str());
        ImGui::Text("%s", m_WindowName.c_str());
        ImGui::End();
    }

    GroundStationWindow * GetPtr() override{
        return this;
    }
};

class DemoWindow2: public GroundStationWindow{
    const string m_WindowName;
public:
    DemoWindow2(string name): m_WindowName(name){

    }
    void RenderGUI() override{
        ImGui::Begin(m_WindowName.c_str());
        ImGui::Text("%s", m_WindowName.c_str());
        ImGui::End();
    }

    GroundStationWindow* GetPtr(){
        return this;
    }
};

class ImPlotDemo : public GroundStationWindow{
    string m_WindowName;
public:
    ImPlotDemo(){
        m_WindowName=" ";
    }

    void RenderGUI() override{
        ImPlot::ShowDemoWindow();
    }

    GroundStationWindow * GetPtr() override{
        return this;
    }
};
#endif //GSW_MODULES_H
