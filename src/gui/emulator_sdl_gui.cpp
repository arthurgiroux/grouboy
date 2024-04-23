#include "emulator_sdl_gui.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void audioBufferCallbackWrapper(void* userdata, unsigned char* data, int length)
{
    reinterpret_cast<EmulatorSDLGUI*>(userdata)->audioBufferCallback(data, length);
}

EmulatorSDLGUI::EmulatorSDLGUI(Emulator& emulator) : _emulator(emulator)
{
}

void EmulatorSDLGUI::destroy()
{
    if (_isAudioEnabled && _audioDevice >= 0)
    {
        SDL_CloseAudioDevice(_audioDevice);
        _audioDevice = -1;
    }

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

    _window = SDL_CreateWindow("Grouboy", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (_window == nullptr)
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        destroy();
        return false;
    }

    if (_isAudioEnabled)
    {
        // opening an audio device:
        SDL_AudioSpec audio_spec;
        SDL_zero(audio_spec);
        audio_spec.freq = 48000;
        audio_spec.format = AUDIO_F32;
        audio_spec.channels = 2;
        audio_spec.samples = 4800;
        audio_spec.callback = audioBufferCallbackWrapper;
        audio_spec.userdata = this;

        _audioDevice = SDL_OpenAudioDevice(nullptr, 0, &audio_spec, nullptr, 0);

        SDL_PlayAudioDevice(_audioDevice);
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

void EmulatorSDLGUI::audioBufferCallback(unsigned char* data, int length)
{
    if (length <= 0)
    {
        return;
    }

    SDL_memset(data, 0, length);
    {
        std::lock_guard<std::mutex> lk(_soundMutex);
        int bufferSize = _audioBuffer.size();
        if (bufferSize == 0)
        {
            return;
        }

        int nbrSamplesToWrite = length / sizeof(float);
        // If we have less data than the audio buffer then we only write that
        if (bufferSize < nbrSamplesToWrite)
        {
            nbrSamplesToWrite = bufferSize;
        }

        float* buffer = reinterpret_cast<float*>(data);
        for (int i = 0; i < nbrSamplesToWrite; ++i)
        {
            buffer[i] = _audioBuffer.front();
            _audioBuffer.pop();
        }
    }
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

    while (_frameId == _emulator.getPPU().getFrameId())
    {
        _emulator.exec();

        if (_isAudioEnabled)
        {
            auto buffer = _emulator.getAPU().getAudioBuffer();
            if (buffer.size() > 0)
            {
                std::lock_guard<std::mutex> lk(_soundMutex);
                for (int i = 0; i < buffer.size(); ++i)
                {
                    _audioBuffer.push(buffer[i]);
                }

                _emulator.getAPU().resetAudioBuffer();
            }
        }
        _emulator.getAPU().resetAudioBuffer();
    }

    _frameId = _emulator.getPPU().getFrameId();

    auto image = _emulator.getPPU().getLastRenderedFrame();
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
