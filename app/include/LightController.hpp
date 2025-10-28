//
// Created by tanja on 10/27/25.
//

#ifndef LIGHTMANAGER_HPP
#define LIGHTMANAGER_HPP
#include <vector>
#include <engine/core/Controller.hpp>
#include <engine/resources/Shader.hpp>
#include <glm/vec3.hpp>

namespace app {
    struct PointLightData {
        glm::vec3 position;
        bool is_on = false;

        glm::vec3 ambient  = glm::vec3(0.2f, 0.15f, 0.05f); // Dim orange
        glm::vec3 diffuse  = glm::vec3(1.5f, 0.9f, 0.3f);   // Bright orange
        glm::vec3 specular = glm::vec3(1.0f, 0.8f, 0.4f);   // Warm highlight
        float constant     = 1.0f;
        float linear       = 0.5f;
        float quadratic    = 1.0f;
        float brightness_multiplier = 1.0f;
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

    class LightController : public engine::core::Controller {
    public:
        std::string_view name() const override {
            return "app::LightController";
        }

        void set_point_light_brightness(int index, float brightness_multiplier);

    private:
        void initialize() override;

        void update() override;

    public:
        void add_point_light(const glm::vec3 &position);

        void turn_point_light_on(int index);

        void turn_all_point_lights_off();

        bool is_point_light_on(int index) const;

        int point_light_count() const {
            return static_cast<int>(m_point_lights.size());
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

        void toggle_spot_light() {
            m_spot_light.is_on = !m_spot_light.is_on;
        }

        void setup_shader(const engine::resources::Shader *shader) const;

    private:
        bool is_valid_index(int index) const {
            return index >= 0 && index < point_light_count();
        }

        std::vector<PointLightData> m_point_lights{};
        SpotLightData m_spot_light = SpotLightData();
    };
} // app

#endif //LIGHTMANAGER_HPP
