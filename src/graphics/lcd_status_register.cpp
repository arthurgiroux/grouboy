#include "lcd_status_register.hpp"

LCDStatusRegister::LCDStatusRegister(MMU& mmu) : _mmu(mmu)
{
}

void LCDStatusRegister::updateFlagMode(PPU::Mode value)
{
    // We mask the bits used for the mode to reset them
    byte status = _mmu.read(ADDR_LCD_STATUS) & 0b11111100;
    if (value == PPU::HBLANK)
    {
        status |= 0x00;
    }
    else if (value == PPU::VBLANK)
    {
        status |= 0x01;
    }
    else if (value == PPU::OAM_ACCESS)
    {
        status |= 0x02;
    }
    else if (value == PPU::VRAM_ACCESS)
    {
        status |= 0x03;
    }

    _mmu.write(ADDR_LCD_STATUS, status);
}

bool LCDStatusRegister::areLYCAndLYEqual()
{
    return _mmu.read(LY_COMPARE_ADDR) == _mmu.read(ADDR_SCANLINE);
}

void LCDStatusRegister::setLYCompareFlag(bool value)
{
    int status = _mmu.read(ADDR_LCD_STATUS);
    utils::setNthBit(status, 2, value);
    _mmu.write(ADDR_LCD_STATUS, static_cast<byte>(status));
}

bool LCDStatusRegister::isLYCompareStatInterruptEnabled()
{
    return utils::isNthBitSet(_mmu.read(ADDR_LCD_STATUS), 6);
}

bool LCDStatusRegister::isOAMStatInterruptEnabled()
{
    return utils::isNthBitSet(_mmu.read(ADDR_LCD_STATUS), 5);
}

bool LCDStatusRegister::isVBLANKStatInterruptEnabled()
{
    return utils::isNthBitSet(_mmu.read(ADDR_LCD_STATUS), 4);
}

bool LCDStatusRegister::isHBLANKStatInterruptEnabled()
{
    return utils::isNthBitSet(_mmu.read(ADDR_LCD_STATUS), 3);
}
