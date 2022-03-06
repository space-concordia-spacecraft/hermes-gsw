#pragma once

#include <imgui.h>
#include "core/hs_ui_window.h"

namespace hermes {
    class Logger : public UIWindow{

    public:
        Logger();
        void Clear();
        void AddLog(const char* fmt, ...);
        void RenderGUI() override;
        string GetName() const;

    private:
        ImGuiTextBuffer Buf;
        ImGuiTextFilter Filter;
        ImVector<int> LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
        bool AutoScroll;  // Keep scrolling if already at the bottom.

    };
}

