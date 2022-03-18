#pragma once
#include "core/hs_ui_window.h"
#include "implot.h"

namespace hermes {

    class WindowImPlotDemo : public UIWindow {

    public:
        WindowImPlotDemo() {}

        void RenderGUI() override {
            ImPlot::ShowDemoWindow();
        }

        string GetName() const {
            return "ImPlot Demo";
        }

    };

}