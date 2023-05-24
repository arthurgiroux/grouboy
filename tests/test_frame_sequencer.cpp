#include "apu/channels/frame_sequencer.hpp"
#include <gtest/gtest.h>

class FrameSequencerTest : public ::testing::Test
{
  protected:
    static constexpr int BASE_FREQUENCY = 512;
    FrameSequencer frameSequencer = FrameSequencer(BASE_FREQUENCY);
};

class FrameExecutionCounter
{
  public:
    void execute()
    {
        executions++;
    }

    int executions = 0;
};

TEST_F(FrameSequencerTest, BaseFrequencyShouldBeSetCorrectly)
{
    ASSERT_EQ(frameSequencer.getBaseFrequency(), BASE_FREQUENCY);
}

TEST_F(FrameSequencerTest, FrameWithFrequencyOfBaseFrequencyShouldExecuteEverytime)
{
    FrameExecutionCounter counter;
    frameSequencer.addFrame(FrameSequencer::Frame([&counter] { counter.execute(); }, BASE_FREQUENCY));
    for (int i = 0; i < BASE_FREQUENCY * 2; ++i)
    {
        frameSequencer.tick();
        ASSERT_EQ(counter.executions, i + 1);
    }
}

TEST_F(FrameSequencerTest, FrameWithFrequencyOfHalfBaseFrequencyShouldHalfTheTime)
{
    FrameExecutionCounter counter;
    frameSequencer.addFrame(FrameSequencer::Frame([&counter] { counter.execute(); }, BASE_FREQUENCY / 2));
    for (int i = 0; i < BASE_FREQUENCY * 2; ++i)
    {
        frameSequencer.tick();
    }
    ASSERT_EQ(counter.executions, BASE_FREQUENCY);
}

TEST_F(FrameSequencerTest, MultiplesFramesWithFrequencyShouldExecuteCorrectly)
{
    FrameExecutionCounter counterFull;
    FrameExecutionCounter counterHalf;
    frameSequencer.addFrame(FrameSequencer::Frame([&counterHalf] { counterHalf.execute(); }, BASE_FREQUENCY / 2));
    frameSequencer.addFrame(FrameSequencer::Frame([&counterFull] { counterFull.execute(); }, BASE_FREQUENCY));
    for (int i = 0; i < BASE_FREQUENCY * 2; ++i)
    {
        frameSequencer.tick();
    }
    ASSERT_EQ(counterHalf.executions, BASE_FREQUENCY);
    ASSERT_EQ(counterFull.executions, BASE_FREQUENCY * 2);
}