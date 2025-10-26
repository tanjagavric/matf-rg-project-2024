//
// Created by tanja on 10/26/25.
//

#ifndef WORLDBOUNDS_HPP
#define WORLDBOUNDS_HPP
#include <glm/vec3.hpp>
#include <glm/ext/vector_common.hpp>

namespace app {
    class WorldBounds {
    public:
        WorldBounds() = default;

        WorldBounds(const glm::vec3 &min_bound, const glm::vec3 &max_bound) : m_min_bound(min_bound)
                                                                          , m_max_bound(max_bound) {
        }

        const glm::vec3 &min_bound() const {
            return m_min_bound;
        }

        const glm::vec3 &max_bound() const {
            return m_max_bound;
        }

        void set_bounds(const glm::vec3 &min_bound, const glm::vec3 &max_bound);

        void clamp_position(glm::vec3 &position) const;

    private:
        glm::vec3 m_min_bound{};
        glm::vec3 m_max_bound{};
    };
} // app

#endif //WORLDBOUNDS_HPP
