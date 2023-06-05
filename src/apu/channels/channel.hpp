#ifndef GROUBOY_CHANNEL_HPP
#define GROUBOY_CHANNEL_HPP

class Channel
{
  public:
    virtual float getAudioSample() = 0;
    virtual void trigger() = 0;
    bool isEnabled() const;
    virtual ~Channel() = default;

  protected:
    bool _enable = false;
};

#endif // GROUBOY_CHANNEL_HPP
