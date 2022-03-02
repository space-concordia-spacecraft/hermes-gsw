#pragma once

#include "core/ui_window.h"

namespace hermes {
    class WindowCommunication : public UIWindow {

    public:
        WindowCommunication();
        void RenderGUI();
        string GetName() const;

        bool Connect();
        bool Disconnect();

        bool isConnected();

        int Read();
        bool Write();

    };
}
