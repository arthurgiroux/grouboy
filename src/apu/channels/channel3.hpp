#ifndef GROUBOY_CHANNEL3_HPP
#define GROUBOY_CHANNEL3_HPP

#include "apu/signal/wave.hpp"
#include "channel.hpp"
#include "length_timer.hpp"

class Channel3 : public Channel
{
  public:
    Channel3();
    ~Channel3() = default;
    virtual float getAudioSample() override;
    void step(int cycles) override;
    void trigger() override;
    void setWavelength(int wavelength);
    int getWavelength();
    Wave<32>& getWave();
    void setVolumeControl(int value);
    int getVolumeControl() const;

  private:
    static const int LENGTH_TIMER_DURATION = 256;
    int adjustVolume(int value);
    Wave<32> _wave;
    int _wavelength = 0;
    bool _lengthTimerEnabled = false;
    int _volumeCtrl = 0;
};

#endif // GROUBOY_CHANNEL3_HPP
