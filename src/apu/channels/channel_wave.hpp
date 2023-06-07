#ifndef GROUBOY_CHANNEL_WAVE_HPP
#define GROUBOY_CHANNEL_WAVE_HPP

#include "apu/signal/square_wave.hpp"
#include "channel.hpp"
#include "length_timer.hpp"
#include "volume_sweep.hpp"
#include "wavelength_sweep.hpp"

class ChannelWave : public Channel
{
  public:
    ChannelWave();
    virtual ~ChannelWave() = default;
    virtual float getAudioSample() override;
    void step(int cycles) override;
    void trigger() override;
    void setWavelength(int wavelength);
    int getWavelength();
    SquareWave& getWave();
    void setLengthTimer(int timer);
    void enableLengthTimer(bool value);
    bool isLengthTimerEnabled() const;
    void setVolumeControl(int value);
    int getVolumeControl() const;

  protected:
    static const int LENGTH_TIMER_FREQ = 256;
    static const int VOLUME_SWEEP_FREQ = 64;
    void tickLengthTimer();
    virtual void triggerImpl();
    VolumeSweep _volumeSweep;
    SquareWave _squareWave;
    LengthTimer _lengthTimer;
    int _wavelength = 0;
    bool _lengthTimerEnabled = false;
    int _volumeCtrl = 0;
};

#endif // GROUBOY_CHANNEL_WAVE_HPP
