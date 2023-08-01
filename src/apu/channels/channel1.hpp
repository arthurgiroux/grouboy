#ifndef GROUBOY_CHANNEL1_HPP
#define GROUBOY_CHANNEL1_HPP

#include "apu/signal/square_wave.hpp"
#include "channel.hpp"
#include "frame_sequencer.hpp"
#include "frequency_sweep.hpp"
#include "length_timer.hpp"
#include "pulse_channel.hpp"
#include "volume_sweep.hpp"

/**
 * Channel 1 is a pulse audio channel that generates a rectangle waveform.
 * The frequency and the volume can be set directly or controlled through a sweep.
 */
class Channel1 : public PulseChannel
{
  public:
    /**
     * Create a new channel 1
     * @param highpassCoeff The coefficient to use for the DAC filter
     */
    explicit Channel1(float highpassCoeff);

    /**
     * Change the parameter of the frequency sweep.
     * The format of the value is: 0b0CCCBAAA
     * Where    AAA defines the amount of shift to apply to the frequency
     *          B defines the direction of the sweep, 1 decrease and 0 increase the frequency
     *          CCC defines the period at which the sweep will trigger
     *
     * @param value a 8bit value in the format above
     */
    void setFrequencySweepControl(int value);

    /**
     * Get the frequency sweep control value.
     * @see setFrequencySweepControl for details
     * @return The sweep control value
     */
    int getFrequencySweepControl() const;
    ~Channel1() override = default;
    void reset() override;

  protected:
    void triggerImpl() override;

  private:
    /**
     * The frequency at which the frequency sweep is ticked
     */
    static const int FREQUENCY_SWEEP_FREQ = 128;

    /**
     * Function that will be called when the frequency sweep updates the frequency
     * @param frequency The new frequency
     */
    void onFrequencyChanged(int frequency);

    /**
     * Function that will be called when the frequency sweep overflows the frequency
     */
    void onFrequencyOverflow();
    FrequencySweep _frequencySweep;
    int _sweepControlValue = 0;
};

#endif // GROUBOY_CHANNEL1_HPP
