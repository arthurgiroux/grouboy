#ifndef GROUBOY_CHANNEL2_HPP
#define GROUBOY_CHANNEL2_HPP

#include "pulse_channel.hpp"

class Channel2 : public PulseChannel
{
  public:
    explicit Channel2(float highpassCoeff);
    ~Channel2() = default;
};

#endif // GROUBOY_CHANNEL2_HPP
