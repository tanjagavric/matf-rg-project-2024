//
// Created by tanja on 10/19/25.
//

#include "MainController.hpp"

#include <LightController.hpp>
#include <PuzzleController.hpp>
#include <WorldBoundsController.hpp>
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

        m_modelMatrices = new glm::mat4[m_amount];
        srand(static_cast<unsigned int>(time(nullptr)));

        float min_x   = -2.5f;
        float max_x   = 2.5f;
        float min_z   = -13.5f;
        float max_z   = 7.5f;
        float floor_y = -3.0f;

        for (unsigned int i = 0; i < m_amount; i++) {
            glm::mat4 model = glm::mat4(1.0f);

            float x = min_x + static_cast<float>(rand()) / RAND_MAX * (max_x - min_x);
            float z = min_z + static_cast<float>(rand()) / RAND_MAX * (max_z - min_z);

            model = glm::translate(model, glm::vec3(x, floor_y, z));

            model = glm::scale(model, glm::vec3(0.08f));

            m_modelMatrices[i] = model;
        }
    }

    bool MainController::loop() {
        auto platform = get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KEY_ESCAPE).is_down()) {
            return false;
        }
        return true;
    }

    void MainController::update_camera() const {
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
    }

    void MainController::update() {
        auto graphics = get<engine::graphics::GraphicsController>();
        auto light    = get<LightController>();

        light->set_spot_light_position(graphics->camera()->Position);
        light->set_spot_light_direction(graphics->camera()->Front);

        update_camera();
    }

    void MainController::draw_cave() const {
        auto resources        = get<engine::resources::ResourcesController>();
        auto graphics         = get<engine::graphics::GraphicsController>();
        auto light_controller = get<LightController>();

        engine::resources::Model *cave    = resources->model("cave");
        engine::resources::Shader *shader = resources->shader("basic");

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix<>());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::scale(model, glm::vec3(3.0f));
        shader->set_mat4("model", model);

        light_controller->setup_shader(shader);

        shader->set_vec3("viewPos", graphics->camera()->Position);
        shader->set_float("shininess", 4.0f);

        cave->draw(shader);
    }

    void MainController::draw() {
        draw_cave();
        draw_torches();
        draw_rupee();
        draw_skybox();
    }

    void MainController::begin_draw() {
        engine::graphics::OpenGL::clear_buffers();
    }

    void MainController::draw_skybox() const {
        auto resources = get<engine::resources::ResourcesController>();
        auto graphics  = get<engine::graphics::GraphicsController>();
        auto shader    = resources->shader("skybox");
        auto puzzle    = get<PuzzleController>();

        const char *skybox_name = puzzle->should_use_day_skybox() ? "day_skybox" : "night_skybox";
        auto skybox             = resources->skybox(skybox_name);

        graphics->draw_skybox(shader, skybox);
    }

    void MainController::draw_torches() const {
        auto resources                    = get<engine::resources::ResourcesController>();
        auto graphics                     = get<engine::graphics::GraphicsController>();
        auto light_controller             = get<LightController>();
        engine::resources::Model *torch   = resources->model("torch");
        engine::resources::Shader *shader = resources->shader("torch");

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix<>());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        const auto &point_lights = light_controller->all_point_lights();
        for (int i = 0; i < point_lights.size(); i++) {
            glm::vec3 torch_position = point_lights[i].position + glm::vec3(0.0f, -0.5f, 0.0f);

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
            shader->set_bool("lightOn", point_lights[i].is_on);
            torch->draw(shader);
        }
    }

    void MainController::draw_rupee() const {
        auto resources        = get<engine::resources::ResourcesController>();
        auto graphics         = get<engine::graphics::GraphicsController>();
        auto light_controller = get<LightController>();

        engine::resources::Model *rupee   = resources->model("rupee");
        engine::resources::Shader *shader = resources->shader("rupee");

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix<>());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        light_controller->setup_shader(shader);

        shader->set_vec3("viewPos", graphics->camera()->Position);
        shader->set_float("shininess", 32.0f);

        for (unsigned int i = 0; i < m_amount; i++) {
            shader->set_mat4("model", m_modelMatrices[i]);
            rupee->draw(shader);
        }

        rupee->draw(shader);
    }

    void MainController::end_draw() {
        auto platform = get<engine::platform::PlatformController>();
        platform->swap_buffers();
    }
} // app
