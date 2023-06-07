#ifndef GROUBOY_CHANNEL_HPP
#define GROUBOY_CHANNEL_HPP

#include "frame_sequencer.hpp"

class Channel
{
  public:
    explicit Channel();
    virtual float getAudioSample() = 0;
    virtual void trigger() = 0;
    virtual void step(int cycles) = 0;
    bool isEnabled() const;
    virtual ~Channel() = default;
    void tickCounter();

  protected:
    static const int FRAME_SEQUENCER_FREQ = 512;
    bool _enable = false;
    FrameSequencer _frameSequencer;
};

#endif // GROUBOY_CHANNEL_HPP
