
namespace hermes {
    WindowCommunication::WindowCommunication() {

    }

    bool WindowCommunication::Connect() {

    }

    bool WindowCommunication::Disconnect() {

    }

    bool WindowCommunication::isConnected() {

    }

    int WindowCommunication::Read() {

    }

    bool WindowCommunication::Write() {

    }

    string WindowCommunication::GetName() const {
        return "Communication Terminal";
    }

    void WindowCommunication::RenderGUI() override {
        ImPlot::ShowDemoWindow();
    }
}
