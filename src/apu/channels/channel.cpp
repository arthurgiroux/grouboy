#include "channel.hpp"

Channel::Channel(int lengthTimerDuration) : _frameSequencer(FRAME_SEQUENCER_FREQ), _lengthTimer(lengthTimerDuration)
{
    _frameSequencer.addFrame(FrameSequencer::Frame([&] { tickLengthTimer(); }, LENGTH_TIMER_FREQ));
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

void Channel::setLengthTimer(int timer)
{
    _lengthTimer.setStartValue(timer);
}

void Channel::enableLengthTimer(bool value)
{
    _lengthTimerEnabled = value;
}

bool Channel::isLengthTimerEnabled() const
{
    return _lengthTimerEnabled;
}

void Channel::tickLengthTimer()
{
    if (_lengthTimerEnabled && _enable)
    {
        _lengthTimer.tick();
        if (_lengthTimer.isTimerElapsed())
        {
            enable(false);
        }
    }
}