//
// Created by tanja on 10/27/25.
//

#ifndef LIGHTMANAGER_HPP
#define LIGHTMANAGER_HPP
#include <vector>
#include <engine/resources/Shader.hpp>
#include <glm/vec3.hpp>

namespace app {
    struct PointLightData {
        glm::vec3 position;
        bool is_on = true;

        glm::vec3 ambient  = glm::vec3(0.2f, 0.15f, 0.05f); // Dim orange
        glm::vec3 diffuse  = glm::vec3(1.5f, 0.9f, 0.3f);   // Bright orange
        glm::vec3 specular = glm::vec3(1.0f, 0.8f, 0.4f);   // Warm highlight
        float constant     = 1.0f;
        float linear       = 0.5f;
        float quadratic    = 1.0f;
    };

    struct SpotLightData {
        glm::vec3 position;
        glm::vec3 direction;
        bool is_on = true;

        float cutoff_inner = glm::cos(glm::radians(12.5f));
        float cutoff_outer = glm::cos(glm::radians(15.0f));

        glm::vec3 ambient  = glm::vec3(0.6f);
        glm::vec3 diffuse  = glm::vec3(3.0f);
        glm::vec3 specular = glm::vec3(0.5f);
        float constant     = 1.0f;
        float linear       = 0.09f;
        float quadratic    = 0.032f;
    };

    class LightManager {
    public:
        LightManager() = default;

        int add_point_light(const glm::vec3 &position);

        void turn_point_light_on(int index);

        void turn_point_light_off(int index);

        void toggle_point_light(int index);

        bool is_point_light_on(int index) const;

        int point_light_count() const {
            return static_cast<int>(m_point_lights.size());
        }

        PointLightData &point_light(int index) {
            return m_point_lights.at(index);
        }

        const std::vector<PointLightData> &all_point_lights() const {
            return m_point_lights;
        }

        void set_spot_light_position(const glm::vec3 &position) {
            m_spot_light.position = position;
        }

        void set_spot_light_direction(const glm::vec3 &direction) {
            m_spot_light.direction = direction;
        }

        void turn_spot_light_on() {
            m_spot_light.is_on = true;
        }

        void turn_spot_light_off() {
            m_spot_light.is_on = false;
        }

        void toggle_spot_light() {
            m_spot_light.is_on = !m_spot_light.is_on;
        }

        bool is_spot_light_on() const {
            return m_spot_light.is_on;
        }

        SpotLightData &spot_light() {
            return m_spot_light;
        }

        void setup_shader(engine::resources::Shader *shader) const;

    private:
        std::vector<PointLightData> m_point_lights{};
        SpotLightData m_spot_light;
    };
} // app

#endif //LIGHTMANAGER_HPP
