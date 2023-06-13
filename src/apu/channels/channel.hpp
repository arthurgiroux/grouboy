#ifndef GROUBOY_CHANNEL_HPP
#define GROUBOY_CHANNEL_HPP

#include "frame_sequencer.hpp"

class Channel
{
  public:
    explicit Channel();
    virtual ~Channel() = default;
    virtual float getAudioSample() = 0;
    virtual void trigger() = 0;
    virtual void step(int cycles) = 0;
    bool isEnabled() const;
    void enableDAC(bool value);
    void enable(bool value);
    void tickCounter();
    bool isDACEnabled() const;

  protected:
    static const int FRAME_SEQUENCER_FREQ = 512;
    float convertFromDigitalToAnalog(int value);
    bool _enable = false;
    bool _isDACEnabled = false;
    FrameSequencer _frameSequencer;
};

#endif // GROUBOY_CHANNEL_HPP
