//
// Created by tanja on 10/27/25.
//

#ifndef PUZZLEMANAGER_HPP
#define PUZZLEMANAGER_HPP
#include <vector>
#include <engine/core/Controller.hpp>

namespace app {
    class PuzzleController : public engine::core::Controller {
    public:
        std::string_view name() const override {
            return "app::PuzzleController";
        }

        bool should_use_day_skybox() const {
            return m_should_use_day_skybox;
        }

    private:
        void update() override;

        void update_puzzle_timer();

        void update_light_brightness();

        void handle_light_input();

        bool is_sequence_complete() const;

        void turn_light_on(int light_index);

        void record_light_in_sequence(int index);

        bool is_sequence_correct() const;

        bool has_puzzle_timed_out() const;

        void start_puzzle();

        void validate_and_finish_puzzle();

        void solve_puzzle();

        void reset_puzzle();

        bool m_solved                = false;
        bool m_active                = false;
        bool m_should_use_day_skybox = false;
        std::vector<int> m_light_sequence;
        float m_puzzle_timer     = 0.0f;
        float m_brightness_timer = 0.0f;

        static constexpr std::array<int, 4> CORRECT_SEQUENCE = {3, 1, 0, 2}; // 4, 2, 1, 3 (0-indexed)
        static constexpr float PUZZLE_TIME_LIMIT             = 10.0f;
    };
} // app

#endif //PUZZLEMANAGER_HPP
