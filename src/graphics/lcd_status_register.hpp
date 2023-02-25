#ifndef GBEMULATOR_LCD_STATUS_REGISTER_HPP
#define GBEMULATOR_LCD_STATUS_REGISTER_HPP

#include "graphics/ppu.hpp"
#include "memory/mmu.hpp"

/**
 * The LCD Status register is used by the PPU to expose the PPU mode,
 * and offers a way to trigger interrupts when the PPU changes mode.
 * It can also trigger an interrupt when the LY is equal to a certain configurable value.
 */
class LCDStatusRegister
{
  public:
    /**
     * Create a new LCD Status register.
     *
     * @param mmu   The MMU to use to read/write the registers
     */
    explicit LCDStatusRegister(MMU& mmu);
    ;

    /**
     * Update the PPU mode information
     *
     * @param value The value of the PPU mode
     */
    void updateFlagMode(PPU::Mode value);

    /**
     * Checks if LY is equal to the configured value for comparison
     *
     * @return True if both values are equal, false otherwise
     */
    bool areLYCAndLYEqual();

    /**
     * Set the value that we want to use to compare LY to
     *
     * @param value     The scanline value that we will use for comparison with LY
     */
    void setLYCompareFlag(bool value);

    /**
     * Checks if the interrupt for comparing LY is enabled
     *
     * @return True if interrupt is enabled, false otherwise
     */
    bool isLYCompareStatInterruptEnabled();

    /**
     * Checks if the interrupt when the PPU modes changes to OAM is enabled
     *
     * @return True if interrupt is enabled, false otherwise
     */
    bool isOAMStatInterruptEnabled();

    /**
     * Checks if the interrupt when the PPU modes changes to VBLANK is enabled
     *
     * @return True if interrupt is enabled, false otherwise
     */
    bool isVBLANKStatInterruptEnabled();

    /**
     * Checks if the interrupt when the PPU modes changes to HBLANK is enabled
     *
     * @return True if interrupt is enabled, false otherwise
     */
    bool isHBLANKStatInterruptEnabled();

  private:
    /**
     * The MMU to use to read/write the registers.
     */
    MMU& _mmu;

    /**
     * The address of the LY compare register.
     */
    static const int LY_COMPARE_ADDR = 0xFF45;

    /**
     * The address of the LCD status register.
     */
    static const int ADDR_LCD_STATUS = 0xFF41;

    /**
     * The address where we store the information of the scanline being rendered.
     */
    static const int ADDR_SCANLINE = 0xFF44;
};

#endif // GBEMULATOR_LCD_STATUS_REGISTER_HPP
