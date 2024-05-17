#include "lcd_status_register.hpp"

LCDStatusRegister::LCDStatusRegister()
{
}

void LCDStatusRegister::updateFlagMode(PPU::Mode value)
{
    // We mask the bits used for the mode to reset them
    byte status = _LCDStatusRegister & 0b11111100;
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

    _LCDStatusRegister = status;
}

bool LCDStatusRegister::areLYCAndLYEqual()
{
    return _lineYCompareRegister == _scanlineRegister;
}

void LCDStatusRegister::setLYCompareFlag(bool value)
{
    utils::setNthBit(_LCDStatusRegister, 2, value);
}

bool LCDStatusRegister::isLYCompareStatInterruptEnabled()
{
    return utils::isNthBitSet(_LCDStatusRegister, 6);
}

bool LCDStatusRegister::isOAMStatInterruptEnabled()
{
    return utils::isNthBitSet(_LCDStatusRegister, 5);
}

bool LCDStatusRegister::isVBLANKStatInterruptEnabled()
{
    return utils::isNthBitSet(_LCDStatusRegister, 4);
}

bool LCDStatusRegister::isHBLANKStatInterruptEnabled()
{
    return utils::isNthBitSet(_LCDStatusRegister, 3);
}

byte LCDStatusRegister::getLineYCompareRegister() const
{
    return _lineYCompareRegister;
}

void LCDStatusRegister::setLineYCompareRegister(byte lineYCompareRegister)
{
    _lineYCompareRegister = lineYCompareRegister;
}

byte LCDStatusRegister::getLcdStatusRegister() const
{
    return _LCDStatusRegister;
}

void LCDStatusRegister::setLcdStatusRegister(byte lcdStatusRegister)
{
    _LCDStatusRegister = lcdStatusRegister;
}

byte LCDStatusRegister::getScanlineRegister() const
{
    return _scanlineRegister;
}

void LCDStatusRegister::setScanlineRegister(byte scanlineRegister)
{
    _scanlineRegister = scanlineRegister;
}
