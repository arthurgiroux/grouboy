#ifndef GROUBOY_CHANNEL4_HPP
#define GROUBOY_CHANNEL4_HPP

#include "apu/signal/noise_signal.hpp"
#include "channel.hpp"

class Channel4 : public Channel
{
  public:
    explicit Channel4(float highpassCoeff);
    void step(int cycles) override;
    void trigger() override;
    void setVolumeControl(byte value);
    byte getVolumeControl() const;
    float getAudioSample() override;
    void setNoiseControl(byte control);
    byte getNoiseControl() const;
    void reset() override;

  private:
    static const int LENGTH_TIMER_DURATION = 64;
    static const int VOLUME_SWEEP_FREQ = 64;
    VolumeSweep _volumeSweep;
    NoiseSignal _noiseSignal;
    byte _volumeCtrl = 0;
    byte _noiseControl = 0;
};

#endif // GROUBOY_CHANNEL4_HPP
