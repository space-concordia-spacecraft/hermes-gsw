#pragma once

#include "core/UIWindow.h"

namespace hermes {

    class ImPlotDemoWindow : public UIWindow {

    public:
        ImPlotDemoWindow() {}

        void RenderGUI() override {
            ImPlot::ShowDemoWindow();
        }

        string GetName() const {
            return "ImPlot Demo";
        }

    };

}