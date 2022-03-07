#pragma once

#include <imgui.h>
#include "core/hs_ui_window.h"

#ifndef HERMES_HS_COMPONENT_CONSOLE_H
#define HERMES_HS_COMPONENT_CONSOLE_H

namespace hermes {

    class ComponentConsole : public UIWindow {

    public:
        ComponentConsole();
        ~ComponentConsole();
        static int Strnicmp(const char* s1, const char* s2, int n);
        static int Stricmp(const char* s1, const char* s2);
        static char* Strdup(const char* s);
        static void Strtrim(char* s);
        void ClearLog();
        void AddLog(const char* fmt, ...);
        void RenderGUI() override;
        void ExecCommand(const char* command_line);
        static int TextEditCallbackStub(ImGuiInputTextCallbackData* data);
        int TextEditCallback(ImGuiInputTextCallbackData* data);
        string GetName() const override{return "Component Console";};

    private:
        char InputBuf[256];
        ImVector<char*> Items;
        ImVector<const char*> Commands;
        ImVector<char*> History;
        int HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
        ImGuiTextFilter Filter;
        bool AutoScroll;
        bool ScrollToBottom;
    };



}


#endif //HERMES_HS_COMPONENT_CONSOLE_H