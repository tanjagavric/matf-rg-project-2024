//
// Created by tanja on 10/19/25.
//

#include "MainController.hpp"

#include <WorldBounds.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/ResourcesController.hpp>
#include <spdlog/spdlog.h>

namespace app {
    class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
    public:
        void on_mouse_move(engine::platform::MousePosition position) override;
    };

    void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
        auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
        camera->rotate_camera(position.dx, position.dy);
    }

    void MainController::initialize() {
        auto platform = get<engine::platform::PlatformController>();
        platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
        engine::graphics::OpenGL::enable_depth_testing();

        auto graphics            = get<engine::graphics::GraphicsController>();
        auto camera              = graphics->camera();
        camera->MouseSensitivity = 0.3f;

        m_world_bounds = WorldBounds({-2.5f, 0.0f, -13.0f},
                                     {2.5f, 2.5f, 7.5f});
    }

    bool MainController::loop() {
        auto platform = get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KEY_ESCAPE).is_down()) {
            return false;
        }
        return true;
    }

    void MainController::update_camera() {
        auto platform = get<engine::platform::PlatformController>();
        auto graphics = get<engine::graphics::GraphicsController>();
        auto camera   = graphics->camera();
        float dt      = platform->dt();

        if (platform->key(engine::platform::KEY_W).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt);
        }
        if (platform->key(engine::platform::KEY_S).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt);
        }
        if (platform->key(engine::platform::KEY_A).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt);
        }
        if (platform->key(engine::platform::KEY_D).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt);
        }

        m_world_bounds.clamp_position(camera->Position);
    }

    void MainController::update() {
        auto platform = get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KEY_E).state() == engine::platform::Key::State::JustPressed) {
            on_puzzle_solved();
        }
        update_camera();
    }

    void MainController::setup_point_lights(engine::resources::Shader *shader) {
        glm::vec3 torch_ambient  = glm::vec3(0.2f, 0.15f, 0.05f); // Dim orange
        glm::vec3 torch_diffuse  = glm::vec3(1.5f, 0.9f, 0.3f);   // Bright orange
        glm::vec3 torch_specular = glm::vec3(1.0f, 0.8f, 0.4f);   // Warm highlight

        for (int i = 0; i < 4; i++) {
            std::string prefix = "pointLights[" + std::to_string(i) + "]";

            shader->set_vec3(prefix + ".position", m_point_light_positions[i]);
            shader->set_vec3(prefix + ".ambient", torch_ambient);
            shader->set_vec3(prefix + ".diffuse", torch_diffuse);
            shader->set_vec3(prefix + ".specular", torch_specular);
            shader->set_float(prefix + ".constant", 1.0f);
            shader->set_float(prefix + ".linear", 0.5f);
            shader->set_float(prefix + ".quadratic", 1.0f);
        }
    }

    void MainController::draw_cave() {
        auto resources                    = get<engine::resources::ResourcesController>();
        auto graphics                     = get<engine::graphics::GraphicsController>();
        engine::resources::Model *cave    = resources->model("cave");
        engine::resources::Shader *shader = resources->shader("basic");

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix<>());
        shader->set_mat4("view", graphics->camera()->view_matrix());
        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::scale(model, glm::vec3(3.0f));
        shader->set_mat4("model", model);

        setup_point_lights(shader);

        shader->set_vec3("spotLight.position", graphics->camera()->Position);
        shader->set_vec3("spotLight.direction", graphics->camera()->Front);

        shader->set_vec3("spotLight.ambient", glm::vec3(0.6f));
        shader->set_vec3("spotLight.diffuse", glm::vec3(3.0f));
        shader->set_vec3("spotLight.specular", glm::vec3(0.5f));

        shader->set_float("spotLight.constant", 1.0f);
        shader->set_float("spotLight.linear", 0.09f);
        shader->set_float("spotLight.quadratic", 0.032f);
        shader->set_float("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        shader->set_float("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        shader->set_vec3("viewPos", graphics->camera()->Position);
        shader->set_float("shininess", 4.0f);

        cave->draw(shader);
    }

    void MainController::draw() {
        draw_cave();
        draw_torches();
        draw_skybox();
    }

    void MainController::begin_draw() {
        engine::graphics::OpenGL::clear_buffers();
    }

    void MainController::draw_skybox() {
        auto resources = get<engine::resources::ResourcesController>();
        auto skybox    = resources->skybox("night_skybox");

        auto shader   = resources->shader("skybox");
        auto graphics = get<engine::graphics::GraphicsController>();
        graphics->draw_skybox(shader, skybox);
    }

    void MainController::draw_torches() {
        auto resources                    = get<engine::resources::ResourcesController>();
        auto graphics                     = get<engine::graphics::GraphicsController>();
        engine::resources::Model *torch   = resources->model("torch");
        engine::resources::Shader *shader = resources->shader("torch");

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix<>());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        for (int i = 0; i < 4; i++) {
            glm::vec3 torch_position = m_point_light_positions[i] + glm::vec3(0.0f, -0.5f, 0.0f);

            glm::mat4 model = glm::mat4(1.0f);
            model           = glm::translate(model, torch_position);
            model           = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            if (torch_position.x < 0.0f) {
                model = glm::rotate(model, glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            } else {
                model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            }
            model = glm::scale(model, glm::vec3(0.02f));

            shader->set_mat4("model", model);
            torch->draw(shader);
        }
    }

    void MainController::end_draw() {
        auto platform = get<engine::platform::PlatformController>();
        platform->swap_buffers();
    }

    void MainController::on_puzzle_solved() {
        spdlog::info("Puzzle solved!");
        glm::vec3 current_min = m_world_bounds.min_bound();
        glm::vec3 current_max = m_world_bounds.max_bound();
        current_min.z -= 10.0f;
        m_world_bounds.set_bounds(current_min, current_max);
    }
} // app
