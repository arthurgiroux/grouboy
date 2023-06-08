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

bool Channel::isDACEnabled() const
{
    return _isDACEnabled;
}

void Channel::enableDAC(bool value)
{
    _isDACEnabled = value;
}

float Channel::convertFromDigitalToAnalog(int value)
{
    if (!_isDACEnabled)
    {
        return 0.0f;
    }

    // We convert a value from [0, 15] to [-1, 1]
    // Note: the slope direction is negative, 0 gets mapped to 1 and 15 to -1
    return -(value / 7.5f - 1.0f);
}

void Channel::enable(bool value)
{
    _enable = value;
}
