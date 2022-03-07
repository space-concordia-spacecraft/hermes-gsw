#pragma once

#include <imgui.h>
#include "core/hs_ui_window.h"

namespace hermes {
    //! \class ComponentLogger
    //! \brief A class that implements logging capabilities
    //!
    //! This class offers a rich set of tools that allows for detailed logging capabilities.
    class ComponentLogger : public UIWindow{

    public:
        //! ComponentLogger constructor
        //!
        //! \param name the name of the window
        ComponentLogger(string name);
        //! \brief Clears the logs
        void Clear();
        //! \brief Adds a new line to the terminal
        void AddLog(const char* fmt, ...);
        //! \brief Renders the GUI for the logger window
        void RenderGUI() override;
        //! \brief Gets the name of the window
        //!
        //! \return Name of the window
        string GetName() const;

    private:
        /// Text buffer for the output on the terminal
        ImGuiTextBuffer m_Buf;

        /// Text filter to filter the logs in the terminal
        ImGuiTextFilter m_Filter;

        /// Line offset
        ImVector<int> m_LineOffsets;

        /// Flag to know if auto scrolling should be enabled
        bool m_AutoScroll;

        /// Name of the window
        string m_Name;

    };
}

