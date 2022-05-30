#pragma once
#ifndef HERMES_HS_WINDOW_THERMAL_H
#define HERMES_HS_WINDOW_THERMAL_H
#endif //HERMES_HS_WINDOW_THERMAL_H

#include <imgui.h>
#include <chrono>
#include <thread>
#include "core/hs_ui_window.h"
#include "components/hs_component_logger.h"
#include "components/hs_component_console.h"

namespace hermes {
    class WindowThermal : public UIWindow {

    public:
        WindowThermal();

        ~WindowThermal();

        void RenderGUI() override;

        string GetName() const;

    private:

        uint64_t mBatteryCellTemp[4];
        uint32_t mSunSensorTemp;
        uint64_t mBusFacesTemp[4];
        uint64_t mReactionWheelTemp;
        uint64_t mPayloadTemp;

        const char *mElec[9] = {"Lithion Batteries:    ", "On-Board Computer :   ", "ADCS Processor Card:  ", "Ext. Radiator Sensor: ",
                                "Deployment Switch:    ", "Electric Power System:", "Solar Cells:          ", "Power Supply System:  ",
                                "S.P Regulators:"};
        const char *mADCS[6] = {"Magnetometer:   ",
                                "Magnetorquer X: ","Magnetorquer Y: ",
                                "Magnetorquer Z: ","Reaction Wheels:",
                                "Sun Sensor:     "};
        const char *mCDH[3] = {"OBC Power Switch:","UHF SWitch:      ","Payload Switch:  "};
        const char *mTTCC[2] = {"UH Transceiver Antenna: ","GOMSpace Nanocom AX-100:"};
        const char *mPayload[1] = {"GOMSpace NanoCam"};
    };
}