#include "channel.hpp"

Channel::Channel(int lengthTimerDuration, float highpassCoeff)
    : _frameSequencer(FRAME_SEQUENCER_FREQ), _lengthTimer(lengthTimerDuration), _highpassFilter(highpassCoeff)
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
    float in = -(value / 7.5f - 1.0f);

    return _highpassFilter.filterValue(in);
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
    if (_lengthTimerEnabled)
    {
        _lengthTimer.tick();
        if (_lengthTimer.isTimerElapsed())
        {
            enable(false);
        }
    }
}

int Channel::getLengthTimerDuration() const
{
    return _lengthTimer.getDuration();
}

void Channel::reset()
{
    _isDACEnabled = false;
    _lengthTimerEnabled = false;
    _lengthTimer.reset();
}