#ifndef GROUBOY_CHANNEL1_HPP
#define GROUBOY_CHANNEL1_HPP

#include "apu/signal/square_wave.hpp"
#include "channel.hpp"
#include "frame_sequencer.hpp"
#include "frequency_sweep.hpp"
#include "length_timer.hpp"
#include "pulse_channel.hpp"
#include "volume_sweep.hpp"

class Channel1 : public PulseChannel
{
  public:
    explicit Channel1(float highpassCoeff);
    void setSweepControl(int value);
    int getSweepControl() const;
    ~Channel1() = default;
    void reset() override;

  protected:
    void triggerImpl() override;

  private:
    static const int FREQUENCY_SWEEP_FREQ = 128;
    void onFrequencyChanged(int frequency);
    void onFrequencyOverflow();
    FrequencySweep _frequencySweep;
    int _sweepControlValue = 0;
};

#endif // GROUBOY_CHANNEL1_HPP
