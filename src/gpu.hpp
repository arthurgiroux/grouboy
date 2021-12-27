#ifndef GBEMULATOR_GPU_HPP
#define GBEMULATOR_GPU_HPP


class GPU {
public:
    GPU();
    ~GPU() = default;

    enum Mode { OAM_ACCESS, VRAM_ACCESS, HBLANK, VBLANK };


    void step(int nbrTicks);

    Mode getMode() const {
        return currentMode;
    }

    int getCurrentScanline() const {
        return currentScanline;
    }

#ifndef UNIT_TESTING
private:
#endif
    void setMode(Mode value) {
        currentMode = value;
        // TODO: Check if we should take into account modulo of ticks
        ticksSpentInCurrentMode = 0;
    };

    void renderScanline();
    void renderFrame();

    int ticksSpentInCurrentMode = 0;
    Mode currentMode = OAM_ACCESS;
    int currentScanline = 0;

    static const int OAM_ACCESS_TICKS = 80;
    static const int VRAM_ACCESS_TICKS = 172;
    static const int  HBLANK_TICKS = 204;
    static const int VBLANK_TICKS = 4560;

    static const int SCREEN_WIDTH = 160;
    static const int SCREEN_HEIGHT = 144;
};


#endif //GBEMULATOR_GPU_HPP
