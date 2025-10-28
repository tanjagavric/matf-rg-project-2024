//
// Created by tanja on 10/26/25.
//

#ifndef WORLDBOUNDS_HPP
#define WORLDBOUNDS_HPP
#include <engine/core/Controller.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/vector_common.hpp>

namespace app {
    class WorldBoundsController : public engine::core::Controller {
    public:
        std::string_view name() const override {
            return "app::WorldBoundsController";
        }

    private:
        void initialize() override;

        void update() override;

    public:
        void extend_front(float distance);

        void clamp_position(glm::vec3 &position) const;

    private:
        glm::vec3 m_min_bound{};
        glm::vec3 m_max_bound{};
    };
} // app

#endif //WORLDBOUNDS_HPP
