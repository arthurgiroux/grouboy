#include "apu/channels/audio_mixer.hpp"
#include "apu/channels/channel.hpp"
#include <gtest/gtest.h>

class DummyChannel : public Channel
{
  public:
    DummyChannel(float sampleValue) : Channel(LENGTH_TIMER_DURATION), _sampleValue(sampleValue)
    {
    }

    float getAudioSample() override
    {
        return _sampleValue;
    };

    void trigger() override
    {
    }

    void enable()
    {
        _enable = true;
    }

    void disable()
    {
        _enable = false;
    }

    void step(int cycles) override
    {
    }

  private:
    static const int LENGTH_TIMER_DURATION = 64;
    float _sampleValue = 0.f;
};

class AudioMixerTest : public ::testing::Test
{
  public:
    AudioMixerTest()
        : channel1(CHANNEL1_VALUE), channel2(CHANNEL2_VALUE), channel3(CHANNEL3_VALUE), channel4(CHANNEL4_VALUE),
          mixer(&channel1, &channel2, &channel3, &channel4)
    {
    }

  protected:
    static constexpr float CHANNEL1_VALUE = 0.1f;
    static constexpr float CHANNEL2_VALUE = 0.2f;
    static constexpr float CHANNEL3_VALUE = 0.3f;
    static constexpr float CHANNEL4_VALUE = 0.4f;
    DummyChannel channel1;
    DummyChannel channel2;
    DummyChannel channel3;
    DummyChannel channel4;
    AudioMixer mixer;
};

TEST_F(AudioMixerTest, GetSampleShouldReturn0WhenAllPanningAreOff)
{
    mixer.setPanningControlLeft(0);
    mixer.setPanningControlRight(0);
    auto sample = mixer.getSample();
    ASSERT_FLOAT_EQ(sample.left, 0.0f);
    ASSERT_FLOAT_EQ(sample.right, 0.0f);
}

TEST_F(AudioMixerTest, VolumeScaleAtMinimumShouldMakeSampleVeryQuiet)
{
    float volumeFactor = 0.1f;
    channel1.enable();
    mixer.setVolumeScaleLeft(0);
    mixer.setVolumeScaleRight(0);
    mixer.setPanningControlLeft(1);
    mixer.setPanningControlRight(1);
    auto sample = mixer.getSample();
    ASSERT_FLOAT_EQ(sample.left, CHANNEL1_VALUE * volumeFactor);
    ASSERT_FLOAT_EQ(sample.right, CHANNEL1_VALUE * volumeFactor);
}

TEST_F(AudioMixerTest, VolumeScaleAtMaximumShouldLeaveSampleUnchanged)
{
    float volumeFactor = 1.0f;
    channel1.enable();
    mixer.setVolumeScaleLeft(7);
    mixer.setVolumeScaleRight(7);
    mixer.setPanningControlLeft(1);
    mixer.setPanningControlRight(1);
    auto sample = mixer.getSample();
    ASSERT_FLOAT_EQ(sample.left, CHANNEL1_VALUE * volumeFactor);
    ASSERT_FLOAT_EQ(sample.right, CHANNEL1_VALUE * volumeFactor);
}

TEST_F(AudioMixerTest, VolumeScaleShouldBeChangedIndependently)
{
    float minVolumeFactor = 0.1f;
    float maxVolumeFactor = 1.0f;
    channel1.enable();
    mixer.setVolumeScaleLeft(0);
    mixer.setVolumeScaleRight(7);
    mixer.setPanningControlLeft(1);
    mixer.setPanningControlRight(1);
    auto sample = mixer.getSample();
    ASSERT_FLOAT_EQ(sample.left, CHANNEL1_VALUE * minVolumeFactor);
    ASSERT_FLOAT_EQ(sample.right, CHANNEL1_VALUE * maxVolumeFactor);
}

TEST_F(AudioMixerTest, PanningChannelsOnBothSideShouldOutputOnLeftAndRight)
{
    channel1.enable();
    channel2.enable();
    channel3.enable();
    channel4.enable();
    mixer.setVolumeScaleLeft(7);
    mixer.setVolumeScaleRight(7);
    mixer.setPanningControlLeft(0b1111);
    mixer.setPanningControlRight(0b1111);
    auto sample = mixer.getSample();
    ASSERT_FLOAT_EQ(sample.left, (CHANNEL1_VALUE + CHANNEL2_VALUE + CHANNEL3_VALUE + CHANNEL4_VALUE) / 4.0f);
    ASSERT_FLOAT_EQ(sample.right, (CHANNEL1_VALUE + CHANNEL2_VALUE + CHANNEL3_VALUE + CHANNEL4_VALUE) / 4.0f);
}

TEST_F(AudioMixerTest, PanningChannelsOnLeftSideShouldOutputOnLeftAndNotRight)
{
    channel1.enable();
    channel2.enable();
    channel3.enable();
    channel4.enable();
    mixer.setVolumeScaleLeft(7);
    mixer.setVolumeScaleRight(7);
    mixer.setPanningControlLeft(0b0001);
    mixer.setPanningControlRight(0b0000);
    auto sample = mixer.getSample();
    ASSERT_FLOAT_EQ(sample.left, (CHANNEL1_VALUE) / 1.0f);
    ASSERT_FLOAT_EQ(sample.right, 0.0f);

    mixer.setPanningControlLeft(0b0010);
    sample = mixer.getSample();
    ASSERT_FLOAT_EQ(sample.left, (CHANNEL2_VALUE) / 1.0f);
    ASSERT_FLOAT_EQ(sample.right, 0.0f);

    mixer.setPanningControlLeft(0b0100);
    sample = mixer.getSample();
    ASSERT_FLOAT_EQ(sample.left, (CHANNEL3_VALUE) / 1.0f);
    ASSERT_FLOAT_EQ(sample.right, 0.0f);

    mixer.setPanningControlLeft(0b1000);
    sample = mixer.getSample();
    ASSERT_FLOAT_EQ(sample.left, (CHANNEL4_VALUE) / 1.0f);
    ASSERT_FLOAT_EQ(sample.right, 0.0f);

    mixer.setPanningControlLeft(0b1111);
    sample = mixer.getSample();
    ASSERT_FLOAT_EQ(sample.left, (CHANNEL1_VALUE + CHANNEL2_VALUE + CHANNEL3_VALUE + CHANNEL4_VALUE) / 4.0f);
    ASSERT_FLOAT_EQ(sample.right, 0.0f);
}

TEST_F(AudioMixerTest, PanningChannelsOnRightSideShouldOutputOnRightAndNotLeft)
{
    channel1.enable();
    channel2.enable();
    channel3.enable();
    channel4.enable();
    mixer.setVolumeScaleLeft(7);
    mixer.setVolumeScaleRight(7);
    mixer.setPanningControlLeft(0b0000);
    mixer.setPanningControlRight(0b0001);
    auto sample = mixer.getSample();
    ASSERT_FLOAT_EQ(sample.right, (CHANNEL1_VALUE) / 1.0f);
    ASSERT_FLOAT_EQ(sample.left, 0.0f);

    mixer.setPanningControlRight(0b0010);
    sample = mixer.getSample();
    ASSERT_FLOAT_EQ(sample.right, (CHANNEL2_VALUE) / 1.0f);
    ASSERT_FLOAT_EQ(sample.left, 0.0f);

    mixer.setPanningControlRight(0b0100);
    sample = mixer.getSample();
    ASSERT_FLOAT_EQ(sample.right, (CHANNEL3_VALUE) / 1.0f);
    ASSERT_FLOAT_EQ(sample.left, 0.0f);

    mixer.setPanningControlRight(0b1000);
    sample = mixer.getSample();
    ASSERT_FLOAT_EQ(sample.right, (CHANNEL4_VALUE) / 1.0f);
    ASSERT_FLOAT_EQ(sample.left, 0.0f);

    mixer.setPanningControlRight(0b1111);
    sample = mixer.getSample();
    ASSERT_FLOAT_EQ(sample.right, (CHANNEL1_VALUE + CHANNEL2_VALUE + CHANNEL3_VALUE + CHANNEL4_VALUE) / 4.0f);
    ASSERT_FLOAT_EQ(sample.left, 0.0f);
}

TEST_F(AudioMixerTest, PanningChannelsDisableChannelsShouldNotOutputAnything)
{
    channel1.disable();
    channel2.disable();
    channel3.disable();
    channel4.disable();
    mixer.setVolumeScaleLeft(7);
    mixer.setVolumeScaleRight(7);
    mixer.setPanningControlLeft(0b1111);
    mixer.setPanningControlRight(0b1111);
    auto sample = mixer.getSample();
    ASSERT_FLOAT_EQ(sample.left, 0.0f);
    ASSERT_FLOAT_EQ(sample.right, 0.0f);
}