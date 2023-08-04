#ifndef GROUBOY_CHANNEL2_HPP
#define GROUBOY_CHANNEL2_HPP

#include "pulse_channel.hpp"

/**
 * Channel 2 is a pulse audio channel that generates a rectangle waveform.
 * The volume can be set directly or controlled through a sweep.
 */
class Channel2 : public PulseChannel
{
  public:
    /**
     * Create a new channel 2
     * @param highpassCoeff The coefficient to use for the DAC filter
     */
    explicit Channel2(float highpassCoeff);
    ~Channel2() override = default;

  private:
    void triggerImpl() override{};
};

#endif // GROUBOY_CHANNEL2_HPP
