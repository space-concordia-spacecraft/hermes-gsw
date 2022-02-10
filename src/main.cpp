#include "core/Application.h"

int main() {
    auto* hermes = new hermes::Application();
    hermes->Run();
    delete hermes;
}