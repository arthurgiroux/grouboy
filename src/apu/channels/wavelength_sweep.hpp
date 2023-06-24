#ifndef GROUBOY_WAVELENGTH_SWEEP_HPP
#define GROUBOY_WAVELENGTH_SWEEP_HPP

#include <functional>

class WavelengthSweep
{
  public:
    typedef std::function<void(int)> WavelengthChangedCallback;
    typedef std::function<void()> WavelengthOverflowCallback;

    explicit WavelengthSweep();
    void tick();
    int getPeriod() const;
    void setPeriod(int period);
    int getDirection() const;
    void setDirection(int sign);
    void setWavelength(int wavelength);
    int getWavelength() const;
    void setShift(int shift);
    int getShift() const;
    void setWavelengthChangedCallback(WavelengthChangedCallback cb);
    void setWavelengthOverflowCallback(WavelengthOverflowCallback cb);
    void reset();
    void trigger();
    void setEnabled(bool value);
    void resetTimer();

  private:
    WavelengthChangedCallback _wavelengthChangedCallback;
    WavelengthOverflowCallback _wavelengthOverflowCallback;
    int generateNewWavelength() const;
    bool didWavelengthOverflow(int wavelength) const;
    int _timer = 0;
    int _period = 0;
    int _wavelength = 0;
    int _sign = 1;
    int _shift = 0;
    bool _enabled = false;
};

#endif // GROUBOY_WAVELENGTH_SWEEP_HPP
