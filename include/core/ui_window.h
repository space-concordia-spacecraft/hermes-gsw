#pragma once

namespace hermes {

    class UIWindow {

    public:
        virtual void RenderGUI() = 0;
        virtual string GetName() const = 0;

    };

}