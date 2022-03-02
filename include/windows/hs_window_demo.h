#pragma once

#include "core/hs_ui_window.h"

namespace hermes {

    class WindowDemo : public UIWindow {

    public:
        WindowDemo(string name)
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

