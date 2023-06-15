#ifndef GROUBOY_CHANNEL_HPP
#define GROUBOY_CHANNEL_HPP

#include "apu/signal/square_wave.hpp"
#include "frame_sequencer.hpp"
#include "length_timer.hpp"
#include "volume_sweep.hpp"
#include "wavelength_sweep.hpp"

class Channel
{
  public:
    explicit Channel(int lengthTimerDuration);
    virtual ~Channel() = default;
    virtual float getAudioSample() = 0;
    virtual void trigger() = 0;
    virtual void step(int cycles) = 0;
    bool isEnabled() const;
    void enableDAC(bool value);
    void enable(bool value);
    void tickCounter();
    bool isDACEnabled() const;
    void setLengthTimer(int timer);
    void enableLengthTimer(bool value);
    bool isLengthTimerEnabled() const;

  protected:
    static const int FRAME_SEQUENCER_FREQ = 512;
    static const int LENGTH_TIMER_FREQ = 256;
    float convertFromDigitalToAnalog(int value);
    void tickLengthTimer();
    bool _enable = false;
    bool _isDACEnabled = false;
    FrameSequencer _frameSequencer;
    LengthTimer _lengthTimer;
    bool _lengthTimerEnabled = false;
};

#endif // GROUBOY_CHANNEL_HPP
