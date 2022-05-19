#include "windows/hs_window_battery.h"

namespace hermes {

    WindowBattery::WindowBattery() {

    }

    WindowBattery::~WindowBattery() {}

    string WindowBattery::GetName() const {
        return "Battery Terminal";
    }
    
    void WindowBattery::RenderGUI() {
        static bool animate = false;
        static bool mTen = false;
        static bool mFourtyfive = false;
        static bool mSeventyfive = false;

        ImGui::Begin(GetName().c_str());
        ImGui::Text("%s", "Battery Status");
        ImGui::ProgressBar(13/15,ImVec2(0.0f, 0.0f));

        ImGui::Checkbox("Animate", &animate);
        ImGui::Checkbox("10%", &mTen);
        ImGui::Checkbox("45%", &mFourtyfive);
        ImGui::Checkbox("75%", &mSeventyfive);

        // Animate a simple progress bar
        static float progress = 0.0f, progress_dir = 1.0f;
        if (animate)
        {
            progress += progress_dir * 0.4f * ImGui::GetIO().DeltaTime;
            if (progress >= +1.1f) { progress = +1.1f; progress_dir *= -1.0f; }
            if (progress <= -0.1f) { progress = -0.1f; progress_dir *= -1.0f; }
        }

        if(mTen && !mFourtyfive && !mSeventyfive){ progress = 0.1f;}
        if(!mTen && mFourtyfive && !mSeventyfive){ progress = 0.45f;}
        if(!mTen && !mFourtyfive && mSeventyfive){ progress = 0.75f;}

        //TODO Colours
        //if(progress > 0.5f) {ImGui::PopStyleColor(1); ImGui::PushStyleColor(ImGuiCol_WindowBg,mColorVectGreen);}
        //if(progress <= 0.5f && progress > 0.1f) {ImGui::PopStyleColor(1); ImGui::PushStyleColor(ImGuiCol_MenuBarBg,mColorVectYellow);}
        //if(progress <= 0.1f) {ImGui::PopStyleColor(1); ImGui::PushStyleColor(ImGuiCol_MenuBarBg,mColorVectRed);}


        // Typically we would use ImVec2(-1.0f,0.0f) to use all available width, or ImVec2(width,0.0f) for a specified width. ImVec2(0.0f,0.0f) uses ItemWidth.
        //Creates a Progress Bar with progress in percentage
        ImGui::ProgressBar(progress, ImVec2(0.0f,0.0f));
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Text("Progress Bar");

        //Creates a Progress Bar with progress compared to a fixed total 1753
        float progress_saturated = (progress < 0.0f) ? 0.0f : (progress > 1.0f) ? 1.0f : progress;
        char buf[32];
        sprintf(buf, "%d/%d", (int)(progress_saturated*1753), 1753);
        ImGui::ProgressBar(progress, ImVec2(0.f,0.f), buf);


        ImGui::End();

    }
}