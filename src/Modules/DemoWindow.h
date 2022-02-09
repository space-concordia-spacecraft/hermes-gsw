#ifndef GSW_DEMOWINDOW_H
#define GSW_DEMOWINDOW_H

#include "../Modules.h"

class DemoWindow : public UIWindowTemplate {
    const string m_WindowName;
public:
    DemoWindow(string name) : m_WindowName(name) {
    }

    void RenderGUI() override {
        ImGui::Begin(m_WindowName.c_str());
        ImGui::Text("%s", m_WindowName.c_str());
        ImGui::End();
    }

    UIWindowTemplate *GetPtr() override {
        return this;
    }

    const string &getMWindowName() const {
        return m_WindowName;
    }
};

class DemoWindow2 : public UIWindowTemplate {
    const string m_WindowName;
public:
    DemoWindow2(string name) : m_WindowName(name) {

    }

    void RenderGUI() override {
        ImGui::Begin(m_WindowName.c_str());
        ImGui::Text("%s", m_WindowName.c_str());
        ImGui::End();
    }

    UIWindowTemplate *GetPtr() {
        return this;
    }
};

#endif //GSW_DEMOWINDOW_H
