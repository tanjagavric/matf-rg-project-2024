//
// Created by tanja on 10/19/25.
//

#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include <WorldBounds.hpp>
#include <engine/core/Controller.hpp>
#include <engine/resources/Shader.hpp>

namespace app {
    class MainController : public engine::core::Controller {
        void initialize() override;

        bool loop() override;

        void update_camera();

        void update() override;

        void setup_point_lights(engine::resources::Shader *shader);

        void draw_cave();

        void begin_draw() override;

        void draw_skybox();

        void draw_torches();

        void draw() override;

        void end_draw() override;

        void on_puzzle_solved();

    public:
        std::string_view name() const override {
            return "app::MainController";
        }

    private:
        WorldBounds m_world_bounds;
        std::array<glm::vec3, 4> m_point_light_positions = {
            glm::vec3(-3.7f, 1.2f, -10.0f),
            glm::vec3(-3.5f, 1.3f, 2.0f),
            glm::vec3(4.6f, 1.1f, -6.0f),
            glm::vec3(3.3f, 1.4f, 5.0f)
        };
    };
} // app

#endif //MAINCONTROLLER_HPP
