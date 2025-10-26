//
// Created by tanja on 10/26/25.
//

#include "WorldBounds.hpp"

#include <spdlog/spdlog.h>

namespace app {
    void WorldBounds::set_bounds(const glm::vec3 &min_bound, const glm::vec3 &max_bound) {
        if (min_bound.x >= max_bound.x || min_bound.y >= max_bound.y || min_bound.z >= max_bound.z) {
            spdlog::warn("Invalid movement bounds! Min must be less than max.");
            return;
        }

        m_min_bound = min_bound;
        m_max_bound = max_bound;
    }

    void WorldBounds::clamp_position(glm::vec3 &position) const {
        position.x = glm::clamp(position.x, m_min_bound.x, m_max_bound.x);
        position.y = glm::clamp(position.y, m_min_bound.y, m_max_bound.y);
        position.z = glm::clamp(position.z, m_min_bound.z, m_max_bound.z);
    }
} // app
