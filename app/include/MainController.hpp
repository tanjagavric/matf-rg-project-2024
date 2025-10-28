//
// Created by tanja on 10/19/25.
//

#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include <engine/core/Controller.hpp>
#include <engine/platform/PlatformController.hpp>

namespace app {
    class MainController : public engine::core::Controller {
    public:
        std::string_view name() const override {
            return "app::MainController";
        }

    private:
        void initialize() override;

        bool loop() override;

        void update_camera() const;

        void handle_flashlight_input();

        void update() override;

        void draw_cave() const;

        void begin_draw() override;

        void draw_skybox() const;

        void draw_torches() const;

        void draw() override;

        void end_draw() override;

        void reset_puzzle();
    };
} // app

#endif //MAINCONTROLLER_HPP
