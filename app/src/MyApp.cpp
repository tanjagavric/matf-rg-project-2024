#include "MyApp.hpp"

#include <LightController.hpp>
#include <MainController.hpp>
#include <PuzzleController.hpp>
#include <WorldBoundsController.hpp>
#include <spdlog/spdlog.h>

namespace app {
    void MyApp::app_setup() {
        spdlog::info("Hello, MyApp::app_setup");
        auto main_controller         = register_controller<MainController>();
        auto world_bounds_controller = register_controller<WorldBoundsController>();
        auto puzzle_controller       = register_controller<PuzzleController>();
        auto light_controller        = register_controller<LightController>();
        main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
        light_controller->after(main_controller);
        world_bounds_controller->after(light_controller);
        puzzle_controller->after(world_bounds_controller);
    }
} // app
