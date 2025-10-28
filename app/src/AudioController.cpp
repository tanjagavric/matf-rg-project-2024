//
// Created by tanja on 10/28/25.
//
#define MINIAUDIO_IMPLEMENTATION
#include "AudioController.hpp"

namespace app {
    void AudioController::initialize() {
        ma_engine_init(nullptr, &m_audio_engine_);
        ma_sound_init_from_file(&m_audio_engine_, "resources/audio/zeldas_theme.mp3",
                                MA_SOUND_FLAG_STREAM,
                                nullptr,
                                nullptr,
                                &m_music_);
        ma_sound_set_looping(&m_music_, MA_TRUE);
        ma_sound_set_volume(&m_music_, 0.3f);
        ma_sound_start(&m_music_);
    }

    void AudioController::terminate() {
        ma_sound_uninit(&m_music_);
        ma_engine_uninit(&m_audio_engine_);
    }
} // app
