#ifndef GROUBOY_CHANNEL1_HPP
#define GROUBOY_CHANNEL1_HPP

#include "apu/signal/square_wave.hpp"
#include "length_timer.hpp"
#include "volume_sweep.hpp"
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
    SquareWave& getWave();
    void setLengthTimer(int timer);
    void enableLengthTimer(bool value);
    bool isLengthTimerEnabled() const;
    void setVolumeControl(int value);
    int getVolumeControl() const;

  private:
    void onWavelengthChanged(int wavelength);
    void onWavelengthOverflow();
    void triggerSweep();
    int _tickCounter = 0;
    bool _enable = false;
    WavelengthSweep _wavelengthSweep;
    VolumeSweep _volumeSweep;
    SquareWave _squareWave;
    LengthTimer _lengthTimer;
    int _wavelength = 0;
    int _sweepControlValue = 0;
    bool _lengthTimerEnabled = false;
    int _volumeCtrl = 0;
};

#endif // GROUBOY_CHANNEL1_HPP
