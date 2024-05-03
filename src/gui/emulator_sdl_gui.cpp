#include "emulator_sdl_gui.hpp"
#include "spdlog/spdlog.h"

EmulatorSDLGUI::EmulatorSDLGUI(Emulator& emulator)
    : _emulator(emulator), _apu(_emulator.getAPU()), _ppu(_emulator.getPPU())
{
}

void EmulatorSDLGUI::destroy()
{
    if (_texture != nullptr)
    {
        SDL_DestroyTexture(_texture);
        _texture = nullptr;
    }
    if (_renderer != nullptr)
    {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }
    if (_window != nullptr)
    {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }

    SDL_Quit();
}

bool EmulatorSDLGUI::create()
{
    int flags = SDL_INIT_VIDEO;
    if (_isAudioEnabled)
    {
        flags |= SDL_INIT_AUDIO;
    }

    if (SDL_Init(flags) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        destroy();
        return false;
    }

    _window = SDL_CreateWindow("Grouboy", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (_window == nullptr)
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        destroy();
        return false;
    }

    if (_isAudioEnabled)
    {
        SDL_AudioSpec audio_spec;
        SDL_zero(audio_spec);
        audio_spec.freq = 44100;
        audio_spec.format = SDL_AUDIO_F32;
        audio_spec.channels = 2;

        _audioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_OUTPUT, &audio_spec, nullptr, nullptr);
        if (_audioStream == nullptr)
        {
            spdlog::error("Couldn't open audio stream. Error is: {}", SDL_GetError());
            destroy();
            return false;
        }
        SDL_ResumeAudioDevice(SDL_GetAudioStreamDevice(_audioStream));
    }

    _renderer = SDL_CreateRenderer(_window, nullptr, SDL_RENDERER_PRESENTVSYNC);
    if (_renderer == nullptr)
    {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        destroy();
        return false;
    }

    _texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, PPU::SCREEN_WIDTH,
                                 PPU::SCREEN_HEIGHT);
    if (_texture == nullptr)
    {
        std::cerr << "Main texture could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        destroy();
        return false;
    }

    return true;
}

void EmulatorSDLGUI::mainLoop()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_EVENT_QUIT)
        {
            _shouldQuit = true;
        }
        else if (e.type == SDL_EVENT_KEY_DOWN)
        {
            int keycode = e.key.keysym.sym;
            if (_buttonMapping.count(keycode))
            {
                _emulator.getInputController().setButtonPressed(_buttonMapping[keycode]);
            }
        }
        else if (e.type == SDL_EVENT_KEY_UP)
        {
            int keycode = e.key.keysym.sym;
            if (_buttonMapping.count(keycode))
            {
                _emulator.getInputController().setButtonReleased(_buttonMapping[keycode]);
            }
        }
    }

    while (_frameId == _ppu.getFrameId())
    {
        _emulator.exec();

        if (_isAudioEnabled)
        {
            auto buffer = _apu.getAudioBuffer();
            if (buffer.size() > 0)
            {
                SDL_PutAudioStreamData(_audioStream, buffer.data(), buffer.size() * sizeof(buffer[0]));
                _apu.resetAudioBuffer();
            }
        }
        else
        {
            _apu.resetAudioBuffer();
        }
    }

    _frameId = _ppu.getFrameId();

    auto image = _ppu.getLastRenderedFrame();
    auto data = image.getData();
    void* pixels = nullptr;
    int pitch = 0;
    SDL_LockTexture(_texture, nullptr, &pixels, &pitch);
    std::copy(data.begin(), data.end(), static_cast<char*>(pixels));
    SDL_UnlockTexture(_texture);
    SDL_RenderTexture(_renderer, _texture, nullptr, nullptr);
    SDL_RenderPresent(_renderer);
}

bool EmulatorSDLGUI::shouldQuit() const
{
    return _shouldQuit;
}

void EmulatorSDLGUI::enableAudio(bool status)
{
    _isAudioEnabled = status;
}
