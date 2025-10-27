//
// Created by tanja on 10/27/25.
//

#include "LightManager.hpp"

namespace app {
    int LightManager::add_point_light(const glm::vec3 &position) {
        PointLightData light;
        light.position = position;
        m_point_lights.push_back(light);
        return point_light_count() - 1;
    }

    void LightManager::turn_point_light_on(int index) {
        if (index >= 0 && index < point_light_count()) {
            m_point_lights[index].is_on = true;
        }
    }

    void LightManager::turn_point_light_off(int index) {
        if (index >= 0 && index < point_light_count()) {
            m_point_lights[index].is_on = false;
        }
    }

    void LightManager::toggle_point_light(int index) {
        if (index >= 0 && index < point_light_count()) {
            m_point_lights[index].is_on = !m_point_lights[index].is_on;
        }
    }

    bool LightManager::is_point_light_on(int index) const {
        if (index >= 0 && index < point_light_count()) {
            return m_point_lights[index].is_on;
        }
        return false;
    }

    void LightManager::setup_shader(engine::resources::Shader *shader) const {
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
