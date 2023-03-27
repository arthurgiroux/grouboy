#include "emulator.hpp"
#include <SDL3/SDL.h>
#include <iostream>
#include <map>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* args[])
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;

    if (argc < 2)
    {
        return EXIT_FAILURE;
    }

    Emulator emulator;
    if (!emulator.getMMU().loadCartridge(args[1]))
    {
        return EXIT_FAILURE;
    }

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        // Create window
        window = SDL_CreateWindow("Grouboy", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
        if (window == nullptr)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            // the representation of our audio device in SDL:
            SDL_AudioDeviceID audio_device;

            // opening an audio device:
            SDL_AudioSpec audio_spec;
            SDL_zero(audio_spec);
            audio_spec.freq = 48000;
            audio_spec.format = AUDIO_F32;
            audio_spec.channels = 1;
            audio_spec.samples = 2048;
            audio_spec.callback = NULL;

            audio_device = SDL_OpenAudioDevice(NULL, 0, &audio_spec, NULL, 0);

            SDL_PlayAudioDevice(audio_device);

            renderer = SDL_CreateRenderer(window, nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

            texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, PPU::SCREEN_WIDTH,
                                        PPU::SCREEN_HEIGHT);

            std::map<SDL_Keycode, InputController::Button> buttonMapping = {
                {SDLK_UP, InputController::Button::UP},        {SDLK_DOWN, InputController::Button::DOWN},
                {SDLK_LEFT, InputController::Button::LEFT},    {SDLK_RIGHT, InputController::Button::RIGHT},
                {SDLK_RETURN, InputController::Button::START}, {SDLK_SPACE, InputController::Button::SELECT},
                {SDLK_a, InputController::Button::A},          {SDLK_b, InputController::Button::B},
                {SDLK_RETURN, InputController::Button::START}};

            SDL_Event e;
            bool quit = false;
            int frameId = 0;
            while (!quit)
            {
                while (SDL_PollEvent(&e))
                {
                    if (e.type == SDL_EVENT_QUIT)
                    {
                        quit = true;
                    }
                    else if (e.type == SDL_EVENT_KEY_DOWN)
                    {
                        int keycode = e.key.keysym.sym;
                        if (buttonMapping.count(keycode))
                        {
                            emulator.getInputController().setButtonPressed(buttonMapping[keycode]);
                        }
                    }
                    else if (e.type == SDL_EVENT_KEY_UP)
                    {
                        int keycode = e.key.keysym.sym;
                        if (buttonMapping.count(keycode))
                        {
                            emulator.getInputController().setButtonReleased(buttonMapping[keycode]);
                        }
                    }
                }

                int samplesize = 1024;

                while (frameId == emulator.getPPU().getFrameId())
                {

                    while ((SDL_GetQueuedAudioSize(audio_device)) > samplesize * sizeof(float))
                    {
                        SDL_Delay(1);
                    }

                    emulator.exec();

                    auto audioBuffer = emulator.getAPU().getAudioBuffer();
                    if (audioBuffer.size() > samplesize)
                    {
                        SDL_QueueAudio(audio_device, audioBuffer.data(), audioBuffer.size() * sizeof(float));
                        emulator.getAPU().resetAudioBuffer();
                    }
                }

                frameId = emulator.getPPU().getFrameId();

                auto image = emulator.getPPU().getLastRenderedFrame();
                auto data = image.getData();
                void* pixels = nullptr;
                int pitch = 0;
                SDL_LockTexture(texture, nullptr, &pixels, &pitch);
                std::copy(data.begin(), data.end(), static_cast<char*>(pixels));
                SDL_UnlockTexture(texture);
                SDL_RenderTexture(renderer, texture, nullptr, nullptr);
                SDL_RenderPresent(renderer);
            }

            SDL_CloseAudioDevice(audio_device);
        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
