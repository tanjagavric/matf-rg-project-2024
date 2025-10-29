#include "MyApp.hpp"

#include <AudioController.hpp>
#include <LightController.hpp>
#include <MainController.hpp>
#include <PuzzleController.hpp>
#include <WorldBoundsController.hpp>
#include <spdlog/spdlog.h>

namespace app {
    void MyApp::app_setup() {
        spdlog::info("Hello, MyApp::app_setup");
        auto world_bounds_controller = register_controller<WorldBoundsController>();
        auto main_controller         = register_controller<MainController>();
        auto light_controller        = register_controller<LightController>();
        auto puzzle_controller       = register_controller<PuzzleController>();
        auto audio_controller        = register_controller<AudioController>();
        world_bounds_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
        main_controller->after(world_bounds_controller);
        light_controller->after(main_controller);
        puzzle_controller->after(light_controller);
        audio_controller->after(puzzle_controller);
    }
} // app
