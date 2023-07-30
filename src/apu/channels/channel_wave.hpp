#ifndef GROUBOY_CHANNEL_WAVE_HPP
#define GROUBOY_CHANNEL_WAVE_HPP

#include "apu/signal/square_wave.hpp"
#include "channel.hpp"
#include "frequency_sweep.hpp"
#include "length_timer.hpp"
#include "volume_sweep.hpp"

class ChannelWave : public Channel
{
  public:
    ChannelWave(float highpassCoeff);
    virtual ~ChannelWave() = default;
    virtual float getAudioSample() override;
    void step(int cycles) override;
    void trigger() override;
    void setFrequency(int frequency);
    int getFrequency();
    SquareWave& getWave();
    void setVolumeControl(int value);
    int getVolumeControl() const;
    virtual void reset() override;

  protected:
    static const int VOLUME_SWEEP_FREQ = 64;
    static const int LENGTH_TIMER_DURATION = 64;
    virtual void triggerImpl();
    VolumeSweep _volumeSweep;
    SquareWave _squareWave;
    int _frequency = 0;
    int _volumeCtrl = 0;
};

#endif // GROUBOY_CHANNEL_WAVE_HPP
