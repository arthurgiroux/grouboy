#ifndef GROUBOY_AUDIO_MIXER_HPP
#define GROUBOY_AUDIO_MIXER_HPP

#include "channel.hpp"
#include "memory/mmu.hpp"
#include <array>

/**
 * Class responsible for mixing together multiple mono sound input,
 * coming from 4 channels into one stereo output.
 * It can also dampen the main volume.
 */
class AudioMixer
{
  public:
    /**
     * A stereo sample with float resolution, scale is between [-1, 1]
     */
    class Sample
    {
      public:
        float left;
        float right;
    };

    /**
     * Create a new audio mixer from channel inputs.
     * @param channel1 The first channel
     * @param channel2 The second channel
     * @param channel3 The third channel
     * @param channel4 The fourth channel
     */
    AudioMixer(Channel* channel1, Channel* channel2, Channel* channel3, Channel* channel4);

    /**
     * Get the stereo sample that has been panned and with volume adjusted.
     * @return A stereo sample
     */
    Sample getSample();

    /**
     * Set the scale of the volume for the left output.
     * The scale is an integer between [0, 7].
     * The minimum value will muffle the sound (and not cut it) while the max value will keep it intact.
     * @param scale the scale between 0 and 7
     */
    void setVolumeScaleLeft(int scale);

    /**
     * Set the scale of the volume for the right output.
     * The scale is an integer between [0, 7].
     * The minimum value will muffle the sound (and not cut it) while the max value will keep it intact.
     * @param scale the scale between 0 and 7
     */
    void setVolumeScaleRight(int scale);

    /**
     * Get the scale of the volume for the left output.
     * @return the scale between 0 and 7
     */
    int getVolumeScaleLeft() const;

    /**
     * Get the scale of the volume for the right output.
     * @return the scale between 0 and 7
     */
    int getVolumeScaleRight() const;

    /**
     * Set the panning control for the left output.
     * The panning control is a bitfield where the 4 LSB represents if a channel should be panned or not.
     * For example 0101 will pan channel 1 and 3.
     * @param panningControl The bitfield for which channels to pan
     */
    void setPanningControlLeft(int panningControl);

    /**
     * Set the panning control for the right output.
     * The panning control is a bitfield where the 4 LSB represents if a channel should be panned or not.
     * For example 0101 will pan channel 1 and 3.
     * @param panningControl The bitfield for which channels to pan
     */
    void setPanningControlRight(int panningControl);

    /**
     * Get the panning control for the left output.
     * @return The bitfield for which channels to pan
     */
    int getPanningControlLeft() const;

    /**
     * Get the panning control for the right output.
     * @return The bitfield for which channels to pan
     */
    int getPanningControlRight() const;

    /**
     * Set if the extra output VIN should be mixed in the main left output.
     * @param enable true if it should be mixed, false otherwise
     */
    void enableVinLeft(bool enable);

    /**
     * Set if the extra output VIN should be mixed in the main right output.
     * @param enable true if it should be mixed, false otherwise
     */
    void enableVinRight(bool enable);

    /**
     * Is the extra output VIN mixed in the main left output.
     * @return true if it's mixed, false otherwise
     */
    bool isVinLeftEnabled() const;

    /**
     * Is the extra output VIN mixed in the main right output.
     * @return true if it's mixed, false otherwise
     */
    bool isVinRightEnabled() const;

    /**
     * Reset the audio mixer to its initial state
     */
    void reset();

  private:
    static const int NBR_CHANNELS = 4;
    /*
     * The maximum value that the volume scale can take
     */
    static constexpr float MAX_VOLUME_SCALE_VALUE = 7.f;

    /**
     * The factor applied to the volume when the scale is 0
     */
    static constexpr float MIN_VOLUME_FACTOR = 0.1f;

    /**
     * The factor applied to the volume when the scale is MAX_VOLUME_SCALE_VALUE
     */
    static constexpr float MAX_VOLUME_FACTOR = 1.f;
    std::array<Channel*, NBR_CHANNELS> _channels = {};

    /**
     * Take the current audio sample and pan it according to a panning control bitfield.
     * @param panningControl The bitfield to use to pan
     * @return The panned sample
     */
    float panAudioSample(int panningControl);

    /**
     * Scale the input volume depending on the given scale.
     * @param amplitude The input to scale
     * @param scaleValue The value of the scale to apply
     * @return A volume adjusted output
     */
    float scaleVolume(float amplitude, int scaleValue);
    int _volumeScaleLeft = 0;
    int _volumeScaleRight = 0;
    int _panningControlLeft = 0;
    int _panningControlRight = 0;
    bool _mixVinLeft = false;
    bool _mixVinRight = false;
};

#endif // GROUBOY_AUDIO_MIXER_HPP
