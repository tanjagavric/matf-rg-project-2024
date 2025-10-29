//
// Created by tanja on 10/19/25.
//

#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include <engine/core/Controller.hpp>

namespace app {
    class MainController : public engine::core::Controller {
    public:
        std::string_view name() const override {
            return "app::MainController";
        }

    private:
        void setup_rupee_instances();

        void initialize() override;

        bool loop() override;

        void update_camera() const;

        void update() override;

        void draw_cave() const;

        void begin_draw() override;

        void draw_skybox() const;

        void draw_torches() const;

        void draw_rupees() const;

        void draw_ground() const;

        void draw() override;

        void end_draw() override;
    };
} // app

#endif //MAINCONTROLLER_HPP
