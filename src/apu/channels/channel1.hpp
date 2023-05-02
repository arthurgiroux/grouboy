#ifndef GROUBOY_CHANNEL1_HPP
#define GROUBOY_CHANNEL1_HPP

#include "apu/signal/square_wave.hpp"
#include "wavelength_sweep.hpp"

class Channel1
{
  public:
    explicit Channel1();
    float getAudioSample();
    void step(int cycles);
    void tickCounter();
    void trigger();
    bool isEnabled() const;
    int getWavelength();
    void setWavelength(int wavelength);
    void setSweepControl(int value);
    int getSweepControl() const;

  private:
    void onWavelengthChanged(int wavelength);
    void onWavelengthOverflow();
    void triggerSweep();
    int _tickCounter = 0;
    bool _enable = false;
    int _sweepCounter = 0;
    WavelengthSweep _wavelengthSweep;
    SquareWave _squareWave;
    int _wavelength = 0;
    int _sweepControlValue = 0;
};

#endif // GROUBOY_CHANNEL1_HPP
