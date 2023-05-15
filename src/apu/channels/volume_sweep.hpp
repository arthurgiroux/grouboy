#ifndef GROUBOY_VOLUME_SWEEP_HPP
#define GROUBOY_VOLUME_SWEEP_HPP

class VolumeSweep
{
  public:
    explicit VolumeSweep();
    void setVolume(int volume);
    void setDirection(int sign);
    int setPeriod(int period);
    int getVolume() const;
    void tick();

  private:
    int _volume = 0;
    int _direction = 1;
    int _period = 0;
    int _timer = 0;
};

#endif // GROUBOY_VOLUME_SWEEP_HPP
