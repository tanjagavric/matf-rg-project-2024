//
// Created by tanja on 10/19/25.
//

#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include <WorldBounds.hpp>
#include <engine/core/Controller.hpp>

namespace app {
    class MainController : public engine::core::Controller {
        void initialize() override;

        bool loop() override;

        void update_camera();

        void update() override;

        void draw_cave();

        void begin_draw() override;

        void draw_skybox();

        void draw() override;

        void end_draw() override;

        void on_puzzle_solved();

    public:
        std::string_view name() const override {
            return "app::MainController";
        }

    private:
        WorldBounds m_world_bounds;
    };
} // app

#endif //MAINCONTROLLER_HPP
