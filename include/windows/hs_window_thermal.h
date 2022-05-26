#pragma once
#ifndef HERMES_HS_WINDOW_THERMAL_H
#define HERMES_HS_WINDOW_THERMAL_H
#endif //HERMES_HS_WINDOW_THERMAL_H

#include <imgui.h>
#include <chrono>
#include <thread>
#include "core/hs_ui_window.h"
#include "components/hs_component_logger.h"
#include "components/hs_component_console.h"

namespace hermes {
    class WindowThermal : public UIWindow {

    public:
        WindowThermal();
        ~WindowThermal();
        void RenderGUI() override;
        string GetName() const;
    private:

    };
}
