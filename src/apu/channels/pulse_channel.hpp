#ifndef GROUBOY_PULSE_CHANNEL_HPP
#define GROUBOY_PULSE_CHANNEL_HPP

#include "apu/signal/square_wave.hpp"
#include "channel.hpp"
#include "frequency_sweep.hpp"
#include "length_timer.hpp"
#include "volume_sweep.hpp"

/**
 * A pulse channel is an channel that generates pulse-width modulated waves.
 * The APU has two pulse channels, Channel 1 and Channel 2.
 * This class is an abstract class for the common components of both channels.
 * The frequency of the wave and the output volume can be controlled
 * either directly of through a frequency sweep and volume sweep
 * that will update the frequency and/or automatically periodically.
 */
class PulseChannel : public Channel
{
  public:
    /**
     * Create a new pulse channel
     * @param highpassCoeff The coefficient to use for the DAC filter
     */
    PulseChannel(float highpassCoeff);
    virtual ~PulseChannel() = default;
    float getAudioSample() override;
    void step(int cycles) override;
    void trigger() override;

    /**
     * Set the frequency of the generated channel.
     * The actual frequency of the output signal will be
     * f = CPU_FREQ / (8 x (2048 - X)) Hz
     * Thus, the minimum frequency is 64 Hz and the maximum is 131.1 KHz.
     *
     * @param frequency A value between 0 and 2048
     */
    void setFrequency(int frequency);

    /**
     * Get the frequency of the output wave.
     * @see setFrequency for frequency mapping
     * @return A value between 0 and 2048
     */
    int getFrequency();

    /**
     * Get the generated pulse-width wave
     * @return the square wave output
     */
    SquareWave& getWave();

    /**
     * Set the volume control data.
     * The bit pattern is the following: 0bCCCCBAAA
     *  Where AAA sets the period of the sweep.
     *  B sets the direction of the sweep, where 1 is increasing and 0 is decreasing
     *  CCCC sets the start volume value.
     *
     * @param value a value in the format above
     */
    void setVolumeControl(int value);

    /**
     * Get the volume control value.
     * @see setVolumeControl for details.
     *
     * @return the volume control value
     */
    int getVolumeControl() const;
    void reset() override;

  protected:
    /**
     * The divider frequency for the volume sweep
     */
    static const int VOLUME_SWEEP_FREQ = 64;

    /**
     * The duration of the length timer in ticks
     */
    static const int LENGTH_TIMER_DURATION = 64;

    /**
     * Specific implementation of the trigger mechanism for the
     * channel.
     */
    virtual void triggerImpl() = 0;
    VolumeSweep _volumeSweep;
    SquareWave _squareWave;

    /**
     * The frequency of generated wave
     */
    int _frequency = 0;

    /**
     * The volume control
     */
    int _volumeCtrl = 0;
};

#endif // GROUBOY_PULSE_CHANNEL_HPP
