#include "frame_sequencer.hpp"

FrameSequencer::Frame::Frame(std::function<void()> func, int frequency) : _func(std::move(func)), _frequency(frequency)
{
}

int FrameSequencer::Frame::getFrequency() const
{
    return _frequency;
}

void FrameSequencer::Frame::execute()
{
    _func();
}

FrameSequencer::FrameSequencer(int baseFrequency) : _baseFrequency(baseFrequency)
{
}

void FrameSequencer::addFrame(const FrameSequencer::Frame& frame)
{
    _frames.push_back(frame);
}

void FrameSequencer::tick()
{
    _tickCounter = (_tickCounter + 1) % _baseFrequency;

    for (Frame& frame : _frames)
    {
        int tickRatio = _baseFrequency / frame.getFrequency();
        if (_tickCounter % tickRatio == 0)
        {
            frame.execute();
        }
    }
}

int FrameSequencer::getBaseFrequency() const
{
    return _baseFrequency;
}
