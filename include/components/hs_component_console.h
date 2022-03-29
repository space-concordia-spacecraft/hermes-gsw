#pragma once

#include <utility>
#include <imgui.h>
#include "core/hs_ui_window.h"

namespace hermes {

    class ComponentConsole : public UIWindow {

    public:
        explicit ComponentConsole(string name);
        ~ComponentConsole();

        void RenderGUI() override;

        static int Strnicmp(const char* s1, const char* s2, int n);
        static int Stricmp(const char* s1, const char* s2);
        static char* Strdup(const char* s);
        static void Strtrim(char* s);

        void ClearLog();
        void AddLog(const char* fmt, ...);

        void ExecCommand(const char* command_line);
        static int TextEditCallbackStub(ImGuiInputTextCallbackData* data);
        int TextEditCallback(ImGuiInputTextCallbackData* data);

        string GetName() const override;



    private:
        char m_InputBuf[256]{};
        ImVector<char*> m_Items;
        ImVector<const char*> m_Commands;
        ImVector<char*> m_History;
        int m_HistoryPos;    // -1: new line, 0..m_History.Size-1 browsing history.
        ImGuiTextFilter m_Filter;
        bool m_AutoScroll;
        bool m_ScrollToBottom;
        string m_Name;


    };



}