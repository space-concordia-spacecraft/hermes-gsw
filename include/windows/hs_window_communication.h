#pragma once

#include <imgui.h>
#include "core/hs_ui_window.h"
#include "components/hs_component_logger.h"
#include "components/hs_component_console.h"

namespace hermes {
    class WindowCommunication : public UIWindow {

    public:
        WindowCommunication();
        ~WindowCommunication();
        void RenderGUI() override;
        string GetName() const;

        void Connect(const char* portName);
        bool Disconnect();

        bool IsConnected();

        int Read(char *buffer, unsigned int nbChar);
        bool Write(const char *buffer, unsigned int nbChar);

    private:
        bool connected;
        HANDLE hSerial;
        COMSTAT status;
        DWORD errors;

    };
}



