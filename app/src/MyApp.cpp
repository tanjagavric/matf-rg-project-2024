#include "MyApp.hpp"
#include <spdlog/spdlog.h>

namespace app {
    void MyApp::app_setup() {
        spdlog::info("Hello, MyApp::app_setup");
    }
} // app
