#ifndef GROUBOY_CHANNEL1_HPP
#define GROUBOY_CHANNEL1_HPP

#include "memory/mmu.hpp"
#include "wavelength_sweep.hpp"

class Channel1
{
  public:
    explicit Channel1(MMU* mmu);
    byte getAudioSample();
    void step(int cycles);
    void tickCounter();
    void trigger();

  private:
    static const int SWEEP_REG_ADDR = 0xFF10;
    static const int WAVELENGTH_LOW_REG_ADDR = 0xFF13;
    static const int WAVELENGTH_AND_CONTROL_REG_ADDR = 0xFF14;

    void onWavelengthChanged(int wavelength);
    void onWavelengthOverflow();
    void setWavelength(int wavelength);
    int getWavelength();
    void triggerSweep();
    MMU* _mmu;
    int _tickCounter = 0;
    bool _isEnabled = false;
    int _sweepCounter = 0;
    WavelengthSweep _wavelengthSweep;
    bool _enable = false;
};

#endif // GROUBOY_CHANNEL1_HPP
