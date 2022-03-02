#pragma once

#include "core/ui_window.h"

namespace hermes {

    class DemoWindow : public UIWindow {

    public:
        DemoWindow(string name)
                : m_WindowName(move(name)) {}

        void RenderGUI() override {
            ImGui::Begin(m_WindowName.c_str());
            ImGui::Text("%s", m_WindowName.c_str());
            ImGui::End();
        }

        string GetName() const {
            return m_WindowName;
        }

    private:
        const string m_WindowName;

    };

}

