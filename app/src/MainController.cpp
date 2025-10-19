//
// Created by tanja on 10/19/25.
//

#include "MainController.hpp"

#include <engine/platform/PlatformController.hpp>

namespace app {
    void MainController::initialize() {
    }

    bool MainController::loop() {
        auto platform = get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KEY_ESCAPE).is_down()) {
            return false;
        }
        return true;
    }
} // app
