#pragma once

#include <imgui.h>
#include <chrono>
#include <thread>
#include "core/hs_ui_window.h"
#include "components/hs_component_logger.h"
#include "components/hs_component_console.h"

namespace hermes {
    class WindowBattery : public UIWindow {

    public:
        WindowBattery();
        ~WindowBattery();
        void RenderGUI() override;
        string GetName() const;

    private:
        const ImVec4 mColorVectRed = ImVec4(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 200.0f / 255.0f);
        const ImVec4 mColorVectYellow = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 0.0f / 255.0f, 200.0f / 255.0f);
        const ImVec4 mColorVectGreen = ImVec4(0.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 200.0f / 255.0f);

    };
}