#ifndef GBEMULATOR_GUI_H
#define GBEMULATOR_GUI_H

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl.h"
#include "imgui.h"
#define SDL_MAIN_HANDLED
#include "SDL.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include "SDL_opengl.h"
#endif

#include "emulator.hpp"

/**
 * Graphical User Interface around the emulator.
 * It shows the game window, the tiles and tilemap,
 * the internal state of the cpu and allows control over the emulator (pause, resume, stop).
 */
class GUI
{
  public:
    /**
     * Create the GUI for the given emulator
     * @param emulator the emulator to visualize
     */
    GUI(Emulator& emulator) : emulator(emulator){};
    ~GUI() = default;

    /**
     * Init the GUI,
     * @return false if there was any error.
     */
    bool init();

    /**
     *
     */
    void uninit();

    /**
     *
     */
    void startMainLoop();

  private:
    /**
     * Init the SDL backend for the display interface.
     *
     * @return false if there was any error
     */
    bool initSDL();

    /**
     * Init the ImGUI backend.
     */
    void initImGui();

    /**
     * Init the textures required to display the emulator's renderImGuiFrame.
     */
    void initTextures();

    /**
     * Create a new texture in the OpenGL context
     *
     * @return the id of the created texture
     */
    GLuint createRGBTexture();

    /**
     * Load a RGB image in an OpenGL texture.
     *
     * @param texture_id    the id of the texture to write to
     * @param image         The image to load
     */
    void loadRGBImageInTexture(GLuint texture_id, const RGBImage& image);

    /**
     * Load a RGB image in a subsection of an OpenGL texture.
     *
     * @param texture_id    the id of the texture to write to
     * @param xoffset       the x offset in pixels
     * @param yoffset       the y offset in pixels
     * @param image         the image to load
     */
    void loadRGBImageInSubTexture(GLuint texture_id, int xoffset, int yoffset, const RGBImage& image);

    /**
     * Display the game view.
     * Will show the renderImGuiFrame of the game,
     * controls for the Emulator,
     * and internal states of the CPU.
     */
    void displayGameView();

    /**
     * Display a view of the VRAM content.
     */
    void displayVRAMView();

    /**
     * Display a view of the tile maps.
     */
    void displayTileMapView();

    /**
     * Display a view of the tiles.
     */
    void displayTileView();

    /**
     * Create a new ImGui frame to be rendered later.
     */
    void createImGuiFrame();

    /**
     * Render the current ImGui context.
     */
    void renderImGuiFrame();

    /**
     * Handle input coming from keyboard / mouse.
     */
    void handleInput();

    /**
     * update the input controller with the status of the given key.
     *
     * @param keycode 	the keycode of the key to update
     * @param button 	the button of the input controller that we will map to
     */
    void sendKeyStatusToInputController(int keycode, InputController::Button button);

    Emulator& emulator;
    SDL_GLContext sdlGlContext = nullptr;
    SDL_Window* mainWindow = nullptr;
    std::string glslVersion = "";
    ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    GLuint gameRenderTexture;
    GLuint tileRenderTexture;
    GLuint tileMapRenderTexture;
    bool frameByFrameEnabled = false;
    bool goToNextFrame = true;
    int tileMapId = 0;
    int tileSetId = 0;
    int lastFrameId = 0;
    ImVec2 windowSize = ImVec2(1280, 720);
    ImVec2 tileMapSize =
        ImVec2(PPU::TILEMAP_WIDTH * SingleTile::TILE_WIDTH, PPU::TILEMAP_HEIGHT* SingleTile::TILE_HEIGHT);
    ImVec2 tileViewSize = ImVec2(256, 128);
    int numberOfTileToDisplayPerLine = 16;
    static const int GAMEVIEW_UPSCALE_RATIO = 2;
    enum class VRAMDisplayMode
    {
        BG_MAP,
        TILES
    };
    VRAMDisplayMode currentVRAMDisplayMode = VRAMDisplayMode::BG_MAP;
};

#endif // GBEMULATOR_GUI_H
