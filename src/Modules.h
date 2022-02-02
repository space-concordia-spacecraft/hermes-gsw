#ifndef GSW_MODULES_H
#define GSW_MODULES_H

class GroundStationWindow{
public:
    virtual void renderGUI()=0;
    virtual GroundStationWindow* getPtr()=0;
};

class DemoWindow: public GroundStationWindow{
    const string windowName;
public:
    DemoWindow(string name):windowName(name){
    }
    void renderGUI() override{
        ImGui::Begin(windowName.c_str());
        ImGui::Text("%s", windowName.c_str());
        ImGui::End();
    }

    GroundStationWindow * getPtr() override{
        return this;
    }
};

class DemoWindow2: public GroundStationWindow{
    const string windowName;
public:
    DemoWindow2(string name):windowName(name){

    }
    void renderGUI() override{
        ImGui::Begin(windowName.c_str());
        ImGui::Text("%s", windowName.c_str());
        ImGui::End();
    }

    GroundStationWindow* getPtr(){
        return this;
    }
};

class ImPlotDemo : public GroundStationWindow{
    string windowName;
public:
    ImPlotDemo(){
        windowName=" ";
    }

    void renderGUI() override{
        ImPlot::ShowDemoWindow();
    }

    GroundStationWindow * getPtr() override{
        return this;
    }
};
#endif //GSW_MODULES_H
