#ifndef GROUBOY_CHANNEL1_HPP
#define GROUBOY_CHANNEL1_HPP

#include "apu/signal/square_wave.hpp"
#include "channel.hpp"
#include "frame_sequencer.hpp"
#include "length_timer.hpp"
#include "volume_sweep.hpp"
#include "wavelength_sweep.hpp"

class Channel1 : public Channel
{
  public:
    explicit Channel1();
    void step(int cycles);
    void tickCounter();
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
    virtual float getAudioSample() override;
    virtual void trigger() override;

  private:
    static const int FRAME_SEQUENCER_FREQ = 512;
    static const int LENGTH_TIMER_FREQ = 256;
    static const int WAVELENGTH_SWEEP_FREQ = 128;
    static const int VOLUME_SWEEP_FREQ = 64;
    void onWavelengthChanged(int wavelength);
    void onWavelengthOverflow();
    void triggerSweep();
    void tickLengthTimer();
    WavelengthSweep _wavelengthSweep;
    VolumeSweep _volumeSweep;
    SquareWave _squareWave;
    LengthTimer _lengthTimer;
    FrameSequencer _frameSequencer;
    int _wavelength = 0;
    int _sweepControlValue = 0;
    bool _lengthTimerEnabled = false;
    int _volumeCtrl = 0;
};

#endif // GROUBOY_CHANNEL1_HPP
