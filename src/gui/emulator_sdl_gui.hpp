#ifndef GROUBOY_EMULATOR_SDL_GUI_HPP
#define GROUBOY_EMULATOR_SDL_GUI_HPP

#include "SDL3/SDL.h"
#include "emulator.hpp"
#include <iostream>
#include <map>
#include <mutex>
#include <queue>

class EmulatorSDLGUI
{
  public:
    EmulatorSDLGUI(Emulator& emulator);
    bool create();
    void mainLoop();
    void destroy();
    void enableAudio(bool status);

    /**
     * Function called by SDL when it needs more audio data to play
     * @param data The buffer to fill with available data
     * @param length The number of bytes of the audio buffer to fill
     */
    void audioBufferCallback(unsigned char* data, int length);

    bool shouldQuit() const;

  private:
    Emulator& _emulator;
    SDL_Window* _window = nullptr;
    SDL_Renderer* _renderer = nullptr;
    SDL_Texture* _texture = nullptr;
    int _frameId = 0;
    bool _isAudioEnabled = true;
    SDL_AudioDeviceID _audioDevice = -1;
    bool _shouldQuit = false;

    // Mutex used to have unique access to audio buffer between main thread and SDL audio thread
    std::mutex _soundMutex;
    std::queue<float> _audioBuffer = {};

    std::map<SDL_Keycode, InputController::Button> _buttonMapping = {
        {SDLK_UP, InputController::Button::UP},        {SDLK_DOWN, InputController::Button::DOWN},
        {SDLK_LEFT, InputController::Button::LEFT},    {SDLK_RIGHT, InputController::Button::RIGHT},
        {SDLK_RETURN, InputController::Button::START}, {SDLK_SPACE, InputController::Button::SELECT},
        {SDLK_a, InputController::Button::A},          {SDLK_b, InputController::Button::B},
        {SDLK_RETURN, InputController::Button::START}};
};

#endif // GROUBOY_EMULATOR_SDL_GUI_HPP
