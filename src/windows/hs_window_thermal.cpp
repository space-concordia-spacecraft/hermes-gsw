#include "windows/hs_window_thermal.h"

namespace hermes
{

    WindowThermal::WindowThermal()  {

         mBatteryCellTemp[4] = 0;
         mSunSensorTemp = 0;
         mBusFacesTemp[4] = 0;
         mReactionWheelTemp = 0;
         mPayloadTemp = 0;
    }
    WindowThermal::~WindowThermal()  {

    }

    string WindowThermal::GetName() const {
        return "Thermal Terminal";
    }
    void WindowThermal::RenderGUI() {
        ImGui::Begin(GetName().c_str());
        ImGui::Text("%s", "Temperature of battery cells");
        ImGui::Text("%s", "");

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

        // utility structure for realtime plot
        struct ScrollingBuffer {
            int MaxSize;
            int Offset;
            ImVector<ImVec2> Data;
            ScrollingBuffer(int max_size = 2000) {
                MaxSize = max_size;
                Offset  = 0;
                Data.reserve(MaxSize);
            }
            void AddPoint(float x, float y) {
                if (Data.size() < MaxSize)
                    Data.push_back(ImVec2(x,y));
                else {
                    Data[Offset] = ImVec2(x,y);
                    Offset =  (Offset + 1) % MaxSize;
                }
            }
            void Erase() {
                if (Data.size() > 0) {
                    Data.shrink(0);
                    Offset  = 0;
                }
            }
        };
        // utility structure for realtime plot
        struct RollingBuffer {
            float Span;
            ImVector<ImVec2> Data;
            RollingBuffer() {
                Span = 10.0f;
                Data.reserve(2000);
            }
            void AddPoint(float x, float y) {
                float xmod = fmodf(x, Span);
                if (!Data.empty() && xmod < Data.back().x)
                    Data.shrink(0);
                Data.push_back(ImVec2(xmod, y));
            }
        };

        static ScrollingBuffer sdata1, sdata2;
        static float t = 0;
        t += ImGui::GetIO().DeltaTime;
        sdata1.AddPoint(t, 1.0f);
        sdata2.AddPoint(t, 1.0f);
        static float history = 3.0f;
        ImGui::SliderFloat("History",&history,1,30,"%.1f s");
        static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

        if (ImPlot::BeginPlot("##Scrolling", ImVec2(-1,150))) {
            ImPlot::SetupAxes(NULL, NULL, flags, flags);
            ImPlot::SetupAxisLimits(ImAxis_X1,t - history, t, ImGuiCond_Always);
            ImPlot::SetupAxisLimits(ImAxis_Y1,0,8000);
            ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.6f);
            ImPlot::PlotShaded("Cell #1", &sdata1.Data[0].x, &sdata1.Data[0].y, sdata1.Data.size(), -INFINITY, sdata1.Offset, 2 * sizeof(float));
            ImPlot::PlotLine("Cell #2", &sdata2.Data[0].x, &sdata2.Data[0].y, sdata2.Data.size(), sdata2.Offset, 2*sizeof(float));
            ImPlot::EndPlot();
        }
        ImGui::Text("%s", "Heated-Plate Usage #1");

        static float progress = 0.0f, progress_dir = 1.0f;
        if (animate)
        {
            progress += progress_dir * 0.01f * ImGui::GetIO().DeltaTime ;
            if (progress >= +1.1f) { progress = +1.1f; progress_dir *= -1.0f; }
            if (progress <= -0.1f) { progress = -0.1f; progress_dir *= -1.0f; }
        }

        ImGui::ProgressBar(progress, ImVec2(0.0f,0.0f));

        if (animate)
        {
            progress += progress_dir * 0.2f * ImGui::GetIO().DeltaTime;
            if (progress >= +1.1f) { progress = +1.1f; progress_dir *= -1.0f; }
            if (progress <= -0.1f) { progress = -0.1f; progress_dir *= -1.0f; }
        }
        ImGui::Text("%s", "Heated-Plate Usage #2");

        ImGui::ProgressBar(progress, ImVec2(0.0f,0.0f));

        if(progress <= 0.5){
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Good");
        }
        else if( progress > 0.5f && progress <= 0.75f) {
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Warning");
        }
        else
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Critical");

        ImGui::TextUnformatted("Data :");
        ImGui::Text(" \t PayLoad:        %7i  °C", mPayloadTemp );
        ImGui::Text(" \t Sun Sensor:     %7i  °C ", mSunSensorTemp );
        ImGui::Text(" \t Reaction Wheel: %7i  °C ", mReactionWheelTemp);
        ImGui::Text(" \t Bus Face 1:     %7i  °C ", mBusFacesTemp[1] );
        ImGui::Text(" \t Bus Face 2:     %7i  °C ", mBusFacesTemp[2]);

        ImGui::End();


    }
}

