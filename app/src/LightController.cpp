//
// Created by tanja on 10/27/25.
//

#include "LightController.hpp"

#include <engine/platform/PlatformController.hpp>

namespace app {
    void LightController::initialize() {
        add_point_light(glm::vec3(-3.7f, 1.2f, -10.0f));
        add_point_light(glm::vec3(-3.5f, 1.3f, 2.0f));
        add_point_light(glm::vec3(4.6f, 1.1f, -6.0f));
        add_point_light(glm::vec3(3.3f, 1.4f, 5.0f));
    }

    void LightController::update() {
        auto platform = get<engine::platform::PlatformController>();

        if (platform->key(engine::platform::KEY_F).state() == engine::platform::Key::State::JustPressed) {
            toggle_spot_light();
        }
    }

    void LightController::add_point_light(const glm::vec3 &position) {
        PointLightData light;
        light.position = position;
        m_point_lights.push_back(light);
    }

    void LightController::turn_point_light_on(const int index) {
        if (is_valid_index(index)) {
            m_point_lights[index].is_on = true;
        }
    }

    void LightController::turn_all_point_lights_off() {
        for (auto &light: m_point_lights) {
            light.is_on = false;
        }
    }

    bool LightController::is_point_light_on(int index) const {
        return is_valid_index(index) && m_point_lights[index].is_on;
    }

    void LightController::setup_shader(const engine::resources::Shader *shader) const {
        for (int i = 0; i < point_light_count(); i++) {
            std::string prefix = "pointLights[" + std::to_string(i) + "]";
            const auto &light  = m_point_lights[i];

            shader->set_vec3(prefix + ".position", light.position);

            shader->set_vec3(prefix + ".ambient", light.is_on ? light.ambient : glm::vec3(0.0f));
            shader->set_vec3(prefix + ".diffuse", light.is_on ? light.diffuse : glm::vec3(0.0f));
            shader->set_vec3(prefix + ".specular", light.is_on ? light.specular : glm::vec3(0.0f));

            shader->set_float(prefix + ".constant", light.constant);
            shader->set_float(prefix + ".linear", light.linear);
            shader->set_float(prefix + ".quadratic", light.quadratic);
        }

        shader->set_vec3("spotLight.position", m_spot_light.position);
        shader->set_vec3("spotLight.direction", m_spot_light.direction);

        shader->set_vec3("spotLight.ambient", m_spot_light.is_on ? m_spot_light.ambient : glm::vec3(0.0f));
        shader->set_vec3("spotLight.diffuse", m_spot_light.is_on ? m_spot_light.diffuse : glm::vec3(0.0f));
        shader->set_vec3("spotLight.specular", m_spot_light.is_on ? m_spot_light.specular : glm::vec3(0.0f));

        shader->set_float("spotLight.constant", m_spot_light.constant);
        shader->set_float("spotLight.linear", m_spot_light.linear);
        shader->set_float("spotLight.quadratic", m_spot_light.quadratic);
        shader->set_float("spotLight.cutOff", m_spot_light.cutoff_inner);
        shader->set_float("spotLight.outerCutOff", m_spot_light.cutoff_outer);
    }
} // app
