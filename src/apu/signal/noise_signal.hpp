#ifndef GROUBOY_NOISE_SIGNAL_HPP
#define GROUBOY_NOISE_SIGNAL_HPP

class NoiseSignal
{
  public:
    explicit NoiseSignal();
    void reset();
    void step(int cycles);
    int getAmplitude() const;
    void setFrequency(int frequency);
    int getFrequency() const;
    void enableWideMode(bool enabled);
    bool isWideModeEnabled() const;

  private:
    void nextSample();
    void resetFrequencyTimer();
    int _frequencyTimerValue = 0;
    int _frequency = 0;
    int _lfsrValue = 0;
    bool _wideModeEnabled = false;
};

#endif // GROUBOY_NOISE_SIGNAL_HPP
