#ifndef GROUBOY_CHANNEL_HPP
#define GROUBOY_CHANNEL_HPP

#include "apu/filters/highpass_filter.hpp"
#include "apu/signal/square_wave.hpp"
#include "frame_sequencer.hpp"
#include "frequency_sweep.hpp"
#include "length_timer.hpp"
#include "volume_sweep.hpp"

/**
 * An audio channel is a sound generation unit that is used by the APU
 * to generate sound.
 * Each channel has specificities about the way they generate the sound and
 * this abstraction is about the commonalities of all channels.
 */
class Channel
{
  public:
    /**
     * Create a new audio channel.
     *
     * @param lengthTimerDuration The duration of the length timer
     * @param highpassCoeff     The coefficient to use for the highpass filter of the DAC
     */
    explicit Channel(int lengthTimerDuration, float highpassCoeff);
    virtual ~Channel() = default;

    /**
     * Get the audio sample generated by the channel
     * @return a value between [-1, 1]
     */
    virtual float getAudioSample() = 0;

    /**
     * Trigger the channel, this will effectively enable it and reset part of its generation circuit.
     */
    virtual void trigger() = 0;

    /**
     * Step the generation unit by a certain amount of CPU cycles.
     * @param cycles the number of CPU cycles to step
     */
    virtual void step(int cycles) = 0;

    /**
     * Whether or not the channel is enabled.
     * When a channel is disabled the output is always 0.
     * @return true if enabled, false if disabled.
     */
    bool isEnabled() const;

    /**
     * Enable or disable the DAC.
     * When the DAC is disabled the output of the channel is always 0.
     * @param value true to enable, false to disable
     */
    void enableDAC(bool value);

    /**
     * Returns if the DAC is enabled or not.
     * @return true if enabled, false if disabled.
     */
    bool isDACEnabled() const;

    /**
     * Enable or disable the channel, when a channel is disabled it will not be
     * mixed by the APU at all.
     * @param value true to enable, false to disable
     */
    void enable(bool value);

    /**
     * All channels are ticked by the main gameboy timer to trigger the frame sequencer
     * and some internal sub-circuits.
     */
    void tickCounter();

    /**
     * Set the value of the length timer.
     * The length timer is a mechanism that will disable the channel after a certain time has elapsed.
     *
     * @param timer     a value between 0 and the length timer duration.
     */
    void setLengthTimer(int timer);

    /**
     * Enable or disable the length timer.
     * @param value true to enable, false to disable
     */
    void enableLengthTimer(bool value);

    /**
     * Returns whether or not the length timer is enabled.
     * @return true if enabled, false if disabled.
     */
    bool isLengthTimerEnabled() const;

    /**
     * Get the duration of the length timer
     * @return the duration of the length timer
     */
    int getLengthTimerDuration() const;

    /**
     * Reset the channel to its initial state.
     */
    virtual void reset();

  protected:
    /**
     * The frequency of the frame sequencer
     */
    static const int FRAME_SEQUENCER_FREQ = 512;

    /**
     * The frequency at which the length timer is ticked
     */
    static const int LENGTH_TIMER_FREQ = 256;

    /**
     * Convert a digital value to analog via the DAC
     * @param value a digital value between [0, 15]
     * @return an analog value between [-1, 1]
     */
    float convertFromDigitalToAnalog(int value);

    /**
     * Tick the length timer
     */
    void tickLengthTimer();

    /**
     * Is the channel is enabled or not.
     */
    bool _enable = false;

    /**
     * Is the DAC enabled or not.
     */
    bool _isDACEnabled = false;

    /**
     * The frame sequencer will tick some internal sub-circuit at different frequencies
     */
    FrameSequencer _frameSequencer;

    /**
     * A timer that will disable the channel after a certain duration
     */
    LengthTimer _lengthTimer;

    /**
     * Is the length timer enabled or not.
     */
    bool _lengthTimerEnabled = false;

    /**
     * The high-pass filter that will be used by the DAC
     */
    HighpassFilter _highpassFilter;
};

#endif // GROUBOY_CHANNEL_HPP
