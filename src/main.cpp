#include "core/hs_application.h"
#include "windows/hs_window_battery.h"

int main() {
    auto* hermes = new hermes::Application;
    hermes->Run();

    delete hermes;
}

