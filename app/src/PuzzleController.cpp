//
// Created by tanja on 10/27/25.
//

#include "PuzzleController.hpp"

#include <LightController.hpp>
#include <WorldBoundsController.hpp>
#include <engine/core/Controller.hpp>
#include <engine/platform/PlatformController.hpp>
#include <spdlog/spdlog.h>

namespace app {
    void PuzzleController::update() {
        if (m_solved) {
            update_light_brightness();
            return;
        }

        handle_light_input();
        update_puzzle_timer();
    }

    void PuzzleController::turn_light_on(const int light_index) {
        auto light = get<LightController>();

        // ignore if light is already on
        if (light->is_point_light_on(light_index)) {
            return;
        }

        light->turn_point_light_on(light_index);

        if (!m_active) {
            start_puzzle(); //start if it's the first light press
        }

        record_light_in_sequence(light_index);

        if (is_sequence_complete()) {
            validate_and_finish_puzzle();
        }
    }

    void PuzzleController::handle_light_input() {
        auto platform = get<engine::platform::PlatformController>();

        constexpr std::array light_keys = {
            engine::platform::KEY_1,
            engine::platform::KEY_2,
            engine::platform::KEY_3,
            engine::platform::KEY_4
        };

        for (int i = 0; i < 4; i++) {
            if (platform->key(light_keys[i]).state() == engine::platform::Key::State::JustPressed) {
                turn_light_on(i);
                return;
            }
        }
    }

    bool PuzzleController::is_sequence_complete() const {
        return m_light_sequence.size() == CORRECT_SEQUENCE.size();
    }

    bool PuzzleController::has_puzzle_timed_out() const {
        return m_puzzle_timer >= PUZZLE_TIME_LIMIT;
    }

    void PuzzleController::update_puzzle_timer() {
        if (!m_active) {
            return;
        }

        auto platform = get<engine::platform::PlatformController>();
        m_puzzle_timer += platform->dt();

        if (has_puzzle_timed_out()) {
            spdlog::warn("Puzzle timeout! All lights turned off.");
            reset_puzzle();
        }
    }

    void PuzzleController::update_light_brightness() {
        auto platform = get<engine::platform::PlatformController>();
        auto light    = get<LightController>();

        m_brightness_timer += platform->dt();

        float progress   = std::min(m_brightness_timer / 3.0f, 1.0f);
        float brightness = 1.0f + progress * 5.0f; // Interpolate

        for (int i = 0; i < light->point_light_count(); i++) {
            light->set_point_light_brightness(i, brightness);
        }

        if (m_brightness_timer >= 3.0f) {
            m_should_use_day_skybox = true;
        }
    }

    void PuzzleController::record_light_in_sequence(int index) {
        m_light_sequence.push_back(index);
        spdlog::info("Light {} turned on. Sequence: {} / {}",
                     index + 1,
                     m_light_sequence.size(),
                     CORRECT_SEQUENCE.size());
    }

    bool PuzzleController::is_sequence_correct() const {
        return std::equal(
            m_light_sequence.begin(),
            m_light_sequence.end(),
            CORRECT_SEQUENCE.begin()
        );
    }

    void PuzzleController::validate_and_finish_puzzle() {
        if (is_sequence_correct()) {
            spdlog::info("Correct sequence! Puzzle solved in {:.2f} seconds!", m_puzzle_timer);
            solve_puzzle();
        } else {
            spdlog::warn("Wrong sequence! Try again.");
            reset_puzzle();
        }
    }

    void PuzzleController::solve_puzzle() {
        m_solved           = true;
        m_brightness_timer = 0.0f;
        spdlog::info("Puzzle solved!");

        auto world_bounds = get<WorldBoundsController>();
        world_bounds->extend_front(10.f);
    }

    void PuzzleController::reset_puzzle() {
        m_active       = false;
        m_puzzle_timer = 0.0f;
        m_light_sequence.clear();

        auto light = get<LightController>();
        light->turn_all_point_lights_off();
    }

    void PuzzleController::start_puzzle() {
        m_active       = true;
        m_puzzle_timer = 0.0f;
        m_light_sequence.clear();

        spdlog::info("Puzzle started! You have {} seconds.", PUZZLE_TIME_LIMIT);
    }
} // app
