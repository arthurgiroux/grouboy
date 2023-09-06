#ifndef GROUBOY_CHANNEL3_HPP
#define GROUBOY_CHANNEL3_HPP

#include "apu/signal/wave.hpp"
#include "channel.hpp"
#include "length_timer.hpp"

class Channel3 : public Channel
{
  public:
    Channel3(float highpassCoeff);
    ~Channel3() = default;
    float getAudioSample() override;
    void step(int cycles) override;
    void trigger() override;
    void setFrequency(int frequency);
    int getFrequency();
    Wave<32>& getWave();
    void setVolumeControl(byte value);
    byte getVolumeControl() const;
    void reset() override;

  private:
    static const int LENGTH_TIMER_DURATION = 256;
    int adjustVolume(int value);
    Wave<32> _wave;
    int _frequency = 0;
    bool _lengthTimerEnabled = false;
    byte _volumeCtrl = 0;
};

#endif // GROUBOY_CHANNEL3_HPP
