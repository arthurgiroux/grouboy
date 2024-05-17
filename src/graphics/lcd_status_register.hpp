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
     */
    explicit LCDStatusRegister();

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

    /**
     * Get the value of the line Y compare register.
     * This register value will be compared to the current scanline value when requested.
     * @return a byte value
     */
    byte getLineYCompareRegister() const;

    /**
     * Set the value of the line Y compare register.
     * This register value will be compared to the current scanline value when requested.
     * @param lineYCompareRegister The byte value to compare to the scanline value
     */
    void setLineYCompareRegister(byte lineYCompareRegister);

    /**
     * Get the LCD status register.
     * This register holds several information about the status of the LCD and the PPU.
     * @return the byte value of the LCD status register
     */
    byte getLcdStatusRegister() const;

    /**
     *  Set the LCD status register.
     * This register holds several information about the status of the LCD and the PPU.
     * @param lcdStatusRegister the value to set
     */
    void setLcdStatusRegister(byte lcdStatusRegister);

    /**
     * Get the value of the scanline which might be about to be drawn, being drawn, or just been drawn.
     * @return a value between 0 and 153
     */
    byte getScanlineRegister() const;

    /**
     * Set the value of the scanline being drawn or about to be.
     * @param scanlineRegister a value between 0 and 153
     */
    void setScanlineRegister(byte scanlineRegister);

  private:
    byte _lineYCompareRegister = 0;
    byte _LCDStatusRegister = 0;
    byte _scanlineRegister = 0;
};

#endif // GBEMULATOR_LCD_STATUS_REGISTER_HPP
