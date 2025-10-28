//
// Created by tanja on 10/28/25.
//

#ifndef AUDIOCONTROLLER_HPP
#define AUDIOCONTROLLER_HPP
#include <miniaudio.h>
#include <engine/core/Controller.hpp>

namespace app {
    class AudioController : public engine::core::Controller {
        void initialize() override;

        void terminate() override;

        ma_engine m_audio_engine_ = {};
        ma_sound m_music_         = {};
    };
} // app

#endif //AUDIOCONTROLLER_HPP
