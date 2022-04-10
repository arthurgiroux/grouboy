// Dear ImGui: standalone example application for SDL2 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl3.h"
#include <stdio.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

#include "emulator.hpp"
#include "gpu.hpp"

GLuint createRGBTexture() {
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return image_texture;
}

void loadRawRGBInTexture(GLuint texture_id, int width, int height, const unsigned char* data) {
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
}

void loadRawRGBInSubTexture(GLuint texture_id, int xoffset, int yoffset, int width, int height, const unsigned char* data) {
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
}

// Main code
int main(int, char**)
{
    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
    // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("GameBoy Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    GLuint game_render_texture = createRGBTexture();
    GLuint tile_render_texture = createRGBTexture();
    loadRawRGBInTexture(tile_render_texture, 256, 128, nullptr);

    GLuint tile_map_render_texture = createRGBTexture();
    loadRawRGBInTexture(tile_map_render_texture, 256, 256, nullptr);

    Emulator emulator;

    // Main loop
    bool done = false;
    bool go_to_next_frame = true;
    int last_frame_id = 0;
    int tile_map_id = 0;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        if (go_to_next_frame) {
            while (last_frame_id == emulator.getGPU().getFrameId()) {
                emulator.exec();
            }
            go_to_next_frame = false;
            last_frame_id = emulator.getGPU().getFrameId();
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Game");
            loadRawRGBInTexture(game_render_texture, GPU::SCREEN_WIDTH, GPU::SCREEN_HEIGHT, emulator.getGPU().getCurrentFrame().data());
            ImGui::Image((void*)(intptr_t)game_render_texture, ImVec2(GPU::SCREEN_WIDTH, GPU::SCREEN_HEIGHT));
            ImGui::Text("Frame: %d", last_frame_id);
            ImGui::Text("Ticks: %d", emulator.getCurrentTicks());

            if (ImGui::Button("Next Frame")) {
                go_to_next_frame = true;
            }

            ImGui::Text("Ticks: %d", emulator.getCurrentTicks());

            ImGui::Text("A: %02.2X", emulator.getCPU().getRegisterA());
            ImGui::Text("B: %02.2X", emulator.getCPU().getRegisterB());
            ImGui::Text("C: %02.2X", emulator.getCPU().getRegisterC());
            ImGui::Text("D: %02.2X", emulator.getCPU().getRegisterD());
            ImGui::Text("E: %02.2X", emulator.getCPU().getRegisterE());
            ImGui::Text("H: %02.2X", emulator.getCPU().getRegisterH());
            ImGui::Text("L: %02.2X", emulator.getCPU().getRegisterL());

            ImGui::Text("PC: %04.4X", emulator.getCPU().getProgramCounter());
            ImGui::Text("SP: %04.4X", emulator.getCPU().getStackPointer());

            ImGui::End();
        }

        {
            ImGui::Begin("Tiles");
            for (int x = 0; x < 16; x++) {
                for (int y = 0; y < 16; y++) {
                    int tileId = x * 16 + y;
                    Tile tile = emulator.getGPU().getTileById(tileId, 0);
                    loadRawRGBInSubTexture(tile_render_texture, x * 8, y * 8, 8, 8, tile.toRGB().data());

                    Tile tile2 = emulator.getGPU().getTileById(tileId, 1);
                    loadRawRGBInSubTexture(tile_render_texture, 128 + x * 8,  y * 8, 8, 8, tile.toRGB().data());
                }
            }
            ImGui::Image((void*)(intptr_t)tile_render_texture, ImVec2(256, 128));
            ImGui::End();
        }

        {
            ImGui::Begin("Tile Map");

            if (ImGui::Button("Tile Map 0")) {
                tile_map_id = 0;
            }

            if (ImGui::Button("Tile Map 1")) {
                tile_map_id = 1;
            }

            GPU::TileMap map = emulator.getGPU().getTileMap(tile_map_id);
            for (int x = 0; x < GPU::TILEMAP_WIDTH; x++) {
                for (int y = 0; y < GPU::TILEMAP_HEIGHT; y++) {
                    Tile tile = map[x * 32 + y];
                    loadRawRGBInSubTexture(tile_map_render_texture, x * 8, y * 8, 8, 8, tile.toRGB().data());
                }
            }
            ImGui::Image((void*)(intptr_t)tile_map_render_texture, ImVec2(256, 256));
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}