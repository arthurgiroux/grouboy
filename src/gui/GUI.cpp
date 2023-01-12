#include "GUI.hpp"

bool GUI::init()
{
    bool ret = initSDL();
    if (!ret)
    {
        return false;
    }

    initImGui();
    initTextures();

    return true;
}

GLuint GUI::createRGBTexture()
{
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return image_texture;
}

void GUI::loadRGBImageInTexture(GLuint texture_id, const RGBImage& image)
{
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getWidth(), image.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 image.getData().data());
}

void GUI::loadRGBImageInSubTexture(GLuint texture_id, int xoffset, int yoffset, const RGBImage& image)
{
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, image.getWidth(), image.getHeight(), GL_RGB, GL_UNSIGNED_BYTE,
                    image.getData().data());
}

void GUI::initTextures()
{
    gameRenderTexture = createRGBTexture();
    tileRenderTexture = createRGBTexture();
    loadRGBImageInTexture(tileRenderTexture, RGBImage(128, 256));

    tileMapRenderTexture = createRGBTexture();
    loadRGBImageInTexture(tileMapRenderTexture, RGBImage(tileMapSize.x, tileMapSize.y));
}

void GUI::startMainLoop()
{
    bool shouldExit = false;

    while (!shouldExit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                shouldExit = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(mainWindow))
                shouldExit = true;
        }

        handleInput();

        if (goToNextFrame)
        {
            while (lastFrameId == emulator.getPPU().getFrameId())
            {
                emulator.exec();
            }

            goToNextFrame = !frameByFrameEnabled;
            lastFrameId = emulator.getPPU().getFrameId();
        }

        createImGuiFrame();

        displayGameView();
        displayVRAMView();

        renderImGuiFrame();
    }
}

void GUI::createImGuiFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void GUI::displayVRAMView()
{
    ImGui::Begin("VRAM Display");

    if (ImGui::Button("BG Maps"))
    {
        currentVRAMDisplayMode = VRAMDisplayMode::BG_MAP;
    }

    ImGui::SameLine();

    if (ImGui::Button("Tiles"))
    {
        currentVRAMDisplayMode = VRAMDisplayMode::TILES;
    }

    if (currentVRAMDisplayMode == VRAMDisplayMode::BG_MAP)
    {
        displayTileMapView();
    }

    if (currentVRAMDisplayMode == VRAMDisplayMode::TILES)
    {
        displayTileView();
    }

    ImGui::End();
}

void GUI::displayTileMapView()
{
    if (ImGui::Button("Tile Map 0"))
    {
        tileMapId = 0;
    }

    ImGui::SameLine();

    if (ImGui::Button("Tile Map 1"))
    {
        tileMapId = 1;
    }

    if (ImGui::Button("Tile Set 0"))
    {
        tileSetId = 0;
    }

    ImGui::SameLine();

    if (ImGui::Button("Tile Set 1"))
    {
        tileSetId = 1;
    }

    PPU::TileMap map = emulator.getPPU().getTileMap(tileMapId, tileSetId);
    for (int y = 0; y < PPU::TILEMAP_HEIGHT; y++)
    {
        for (int x = 0; x < PPU::TILEMAP_WIDTH; x++)
        {
            Tile tile = map[y * PPU::TILEMAP_WIDTH + x];
            loadRGBImageInSubTexture(tileMapRenderTexture, x * SingleTile::TILE_WIDTH, y * SingleTile::TILE_HEIGHT,
                                     tile.getImage());
        }
    }
    ImGui::Image((void*)(intptr_t)tileMapRenderTexture, tileMapSize);
}

void GUI::displayTileView()
{
    int xoffsetForTileSet2 = (static_cast<int>(tileViewSize.x) / 2);
    for (int y = 0; y < numberOfTileToDisplayPerLine; y++)
    {
        for (int x = 0; x < numberOfTileToDisplayPerLine; x++)
        {
            int tileId = y * numberOfTileToDisplayPerLine + x;
            Tile tile = emulator.getPPU().getTileById(tileId, 0);
            loadRGBImageInSubTexture(tileRenderTexture, x * SingleTile::TILE_WIDTH, y * SingleTile::TILE_HEIGHT,
                                     tile.getImage());

            Tile tile2 = emulator.getPPU().getTileById(tileId, 1);
            loadRGBImageInSubTexture(tileRenderTexture, xoffsetForTileSet2 + x * SingleTile::TILE_WIDTH,
                                     y * SingleTile::TILE_HEIGHT, tile2.getImage());
        }
    }
    {
        float my_tex_w = tileViewSize.x;
        float my_tex_h = tileViewSize.y;
        ImGuiIO& io = ImGui::GetIO();
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
        ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
        ImGui::Image((void*)(intptr_t)tileRenderTexture, tileViewSize);
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            float region_sz = SingleTile::TILE_HEIGHT;
            int region_x = static_cast<int>((io.MousePos.x - pos.x) / region_sz) * region_sz;
            int region_y = static_cast<int>((io.MousePos.y - pos.y) / region_sz) * region_sz;
            float zoom = 8.0f;
            if (region_x < 0.0f)
            {
                region_x = 0.0f;
            }
            else if (region_x > my_tex_w - region_sz)
            {
                region_x = my_tex_w - region_sz;
            }
            if (region_y < 0.0f)
            {
                region_y = 0.0f;
            }
            else if (region_y > my_tex_h - region_sz)
            {
                region_y = my_tex_h - region_sz;
            }
            ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
            ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
            ImGui::Image((void*)(intptr_t)tileRenderTexture, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1,
                         tint_col, border_col);
            ImGui::EndTooltip();
        }
    }
}

void GUI::displayGameView()
{
    ImGui::Begin("Game");
    loadRGBImageInTexture(gameRenderTexture, emulator.getPPU().getLastRenderedFrame());
    ImGui::BeginGroup();

    ImGui::Image((void*)(intptr_t)gameRenderTexture,
                 ImVec2(PPU::SCREEN_WIDTH * GAMEVIEW_UPSCALE_RATIO, PPU::SCREEN_HEIGHT * GAMEVIEW_UPSCALE_RATIO));

    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();

    if (ImGui::CollapsingHeader("Controls", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (!goToNextFrame && ImGui::Button("Start"))
        {
            goToNextFrame = true;
        }

        else if (goToNextFrame && ImGui::Button("Pause"))
        {
            goToNextFrame = false;
        }

        if (ImGui::Button("Reset"))
        {
            emulator.reset();
            lastFrameId = 0;
        }
    }

    Cartridge* cartridge = emulator.getMMU().getCartridge();
    if (ImGui::CollapsingHeader("Cartridge info", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Title: %s", cartridge != nullptr ? cartridge->getTitle().c_str() : "");
        ImGui::Text("Manufacturer: %s", cartridge != nullptr ? cartridge->getManufacturerCode().c_str() : "");
        ImGui::Text("Type: %s",
                    cartridge != nullptr ? Cartridge::cartridgeTypeToString(cartridge->getType()).c_str() : "");
    }

    if (ImGui::CollapsingHeader("Registers", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("A: %02.2X", emulator.getCPU().getRegisterA());
        ImGui::Text("B: %02.2X", emulator.getCPU().getRegisterB());
        ImGui::Text("C: %02.2X", emulator.getCPU().getRegisterC());
        ImGui::Text("D: %02.2X", emulator.getCPU().getRegisterD());
        ImGui::Text("E: %02.2X", emulator.getCPU().getRegisterE());
        ImGui::Text("H: %02.2X", emulator.getCPU().getRegisterH());
        ImGui::Text("L: %02.2X", emulator.getCPU().getRegisterL());

        ImGui::Text("PC: %04.4X", emulator.getCPU().getProgramCounter());
        ImGui::Text("SP: %04.4X", emulator.getCPU().getStackPointer());
    }

    if (ImGui::CollapsingHeader("Frame info", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Frame: %d", lastFrameId);
        ImGui::Text("Ticks: %d", emulator.getCurrentTicks());
    }

    ImGui::EndGroup();

    ImGui::End();
}

void GUI::renderImGuiFrame()
{
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, static_cast<int>(io.DisplaySize.x), static_cast<int>(io.DisplaySize.y));
    glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(mainWindow);
}

void GUI::uninit()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(sdlGlContext);
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();
}

bool GUI::initSDL()
{
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows
    // systems, depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL
    // is recommended!)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return false;
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    glslVersion = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    glslVersion = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    glslVersion = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags =
        (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    mainWindow = SDL_CreateWindow("GameBoy Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  static_cast<int>(windowSize.x), static_cast<int>(windowSize.y), window_flags);
    sdlGlContext = SDL_GL_CreateContext(mainWindow);
    SDL_GL_MakeCurrent(mainWindow, sdlGlContext);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    return true;
}

void GUI::initImGui()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(mainWindow, sdlGlContext);
    ImGui_ImplOpenGL3_Init(glslVersion.c_str());
}

void GUI::handleInput()
{
    sendKeyStatusToInputController(SDL_SCANCODE_UP, InputController::Button::UP);
    sendKeyStatusToInputController(SDL_SCANCODE_DOWN, InputController::Button::DOWN);
    sendKeyStatusToInputController(SDL_SCANCODE_LEFT, InputController::Button::LEFT);
    sendKeyStatusToInputController(SDL_SCANCODE_RIGHT, InputController::Button::RIGHT);
    sendKeyStatusToInputController(SDL_SCANCODE_A, InputController::Button::A);
    sendKeyStatusToInputController(SDL_SCANCODE_B, InputController::Button::B);
    sendKeyStatusToInputController(SDL_SCANCODE_RETURN, InputController::Button::START);
    sendKeyStatusToInputController(SDL_SCANCODE_S, InputController::Button::SELECT);
}

void GUI::sendKeyStatusToInputController(int keycode, InputController::Button button)
{
    if (ImGui::IsKeyDown(keycode))
    {
        emulator.getInputController().setButtonPressed(button);
    }
    else
    {
        emulator.getInputController().setButtonReleased(button);
    }
}
