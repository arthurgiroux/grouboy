#include "channel.hpp"

Channel::Channel() : _frameSequencer(FRAME_SEQUENCER_FREQ)
{
}

bool Channel::isEnabled() const
{
    return _enable;
}

void Channel::tickCounter()
{
    _frameSequencer.tick();
}