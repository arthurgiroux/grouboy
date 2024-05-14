#ifndef GROUBOY_EMULATOR_SDL_GUI_HPP
#define GROUBOY_EMULATOR_SDL_GUI_HPP

#include "emulator.hpp"
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
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
    bool shouldQuit() const;
    void renderDebugInformation();

  private:
    static const int WINDOW_WIDTH = 640;
    static const int WINDOW_HEIGHT = 480;
    Emulator& _emulator;
    APU& _apu;
    PPU& _ppu;
    SDL_Window* _window = nullptr;
    SDL_Renderer* _renderer = nullptr;
    SDL_Texture* _texture = nullptr;
    TTF_Font* _font = nullptr;
    int _frameId = 0;
    bool _isAudioEnabled = true;
    bool _shouldQuit = false;
    bool _isDebugActivated = false;

    std::map<SDL_Keycode, InputController::Button> _buttonMapping = {
        {SDLK_UP, InputController::Button::UP},        {SDLK_DOWN, InputController::Button::DOWN},
        {SDLK_LEFT, InputController::Button::LEFT},    {SDLK_RIGHT, InputController::Button::RIGHT},
        {SDLK_RETURN, InputController::Button::START}, {SDLK_SPACE, InputController::Button::SELECT},
        {SDLK_a, InputController::Button::A},          {SDLK_b, InputController::Button::B},
        {SDLK_RETURN, InputController::Button::START}};
};

#endif // GROUBOY_EMULATOR_SDL_GUI_HPP
