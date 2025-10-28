//
// Created by tanja on 10/26/25.
//

#include "WorldBoundsController.hpp"

#include <engine/graphics/GraphicsController.hpp>

namespace app {
    void WorldBoundsController::initialize() {
        m_min_bound = {-2.5f, 0.0f, -13.0f};
        m_max_bound = {2.5f, 2.5f, 7.5f};
    }

    void WorldBoundsController::update() {
        auto graphics = get<engine::graphics::GraphicsController>();
        auto camera   = graphics->camera();
        clamp_position(camera->Position);
    }

    void WorldBoundsController::extend_front(float distance) {
        m_min_bound.z -= distance;
    }

    void WorldBoundsController::clamp_position(glm::vec3 &position) const {
        position.x = glm::clamp(position.x, m_min_bound.x, m_max_bound.x);
        position.y = glm::clamp(position.y, m_min_bound.y, m_max_bound.y);
        position.z = glm::clamp(position.z, m_min_bound.z, m_max_bound.z);
    }
} // app
