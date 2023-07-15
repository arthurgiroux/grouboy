#ifndef GROUBOY_CHANNEL2_HPP
#define GROUBOY_CHANNEL2_HPP

#include "channel_wave.hpp"

class Channel2 : public ChannelWave
{
  public:
    explicit Channel2(float highpassCoeff);
    ~Channel2() = default;
};

#endif // GROUBOY_CHANNEL2_HPP
