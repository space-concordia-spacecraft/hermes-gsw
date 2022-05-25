#include "windows/hs_window_thermal.h"

namespace hermes
{

    WindowThermal::WindowThermal()  {

    }
    WindowThermal::~WindowThermal()  {

    }

    string WindowThermal::GetName() const {
        return "Thermal Terminal";
    }
    void WindowThermal::RenderGUI() {
        //temp data needed
        static float Temperature = 0;

        ImGui::Begin(GetName().c_str());
        ImGui::Text("%s", "Temperature");
        ImGui::Text("%s", "");

      /*  static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
        ImGui::PlotLines("Frame Times", arr, IM_ARRAYSIZE(arr));
        */

        float average = 0.0f;

        static bool animate = true;
        static float values[90] = {};
        static int values_offset = 0;
        static double refresh_time = 0.0;
        if (!animate || refresh_time == 0.0)
            refresh_time = ImGui::GetTime();
        while (refresh_time < ImGui::GetTime()) // Create data at fixed 60 Hz rate for the demo
        {
            static float phase = 0.0f;
            values[values_offset] = cosf(phase);
            values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
            phase += 0.10f * values_offset;
            refresh_time += 1.0f / 5.0f;
        }

        for (int n = 0; n < IM_ARRAYSIZE(values); n++)
            average += values[n];
        average /= (float)IM_ARRAYSIZE(values);
        char overlay[32];

        sprintf(overlay, "Current Temp %f", Temperature);

        ImGui::PlotLines("Lines", values, IM_ARRAYSIZE(values), values_offset, overlay, -1.0f, 1.0f, ImVec2(0, 200.0f));




        ImGui::Text("%s", "");
        ImGui::Text("%s", "");

        ImGui::Text("%s", "Heated-Plate Usage");

        static float progress = 0.0f, progress_dir = 1.0f;
        if (animate)
        {
            progress += progress_dir * 0.4f * ImGui::GetIO().DeltaTime;
            if (progress >= +1.1f) { progress = +1.1f; progress_dir *= -1.0f; }
            if (progress <= -0.1f) { progress = -0.1f; progress_dir *= -1.0f; }
        }


        ImGui::ProgressBar(progress, ImVec2(0.0f,0.0f));
        ImGui::End();

     }
    }



