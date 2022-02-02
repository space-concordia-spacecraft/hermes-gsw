#ifndef GSW_IMPLOTDEMO_H
#define GSW_IMPLOTDEMO_H

class ImPlotDemo : public UIWindowTemplate {
    string m_WindowName;
public:
    ImPlotDemo() : m_WindowName(" ") {}

    void RenderGUI() override {
        ImPlot::ShowDemoWindow();
    }

    UIWindowTemplate *GetPtr() override {
        return this;
    }
};

#endif //GSW_IMPLOTDEMO_H
