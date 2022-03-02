#pragma once

#include <imgui.h>

#include "core/hs_ui_window.h"

namespace hermes {
    class WindowCommunication : public UIWindow {

    public:
        WindowCommunication();
        void RenderGUI();
        string GetName() const;

        bool Connect(const char* portName);
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
