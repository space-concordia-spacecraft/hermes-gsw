#pragma once

#include "hs_codes.h"

namespace hermes {

    class UIWindow {

    public:
        virtual void RenderGUI() = 0;
        virtual string GetName() const = 0;

    };

}