
#include "ppu.hpp"
#include "cpu/interrupt_manager.hpp"
#include "graphics/lcd_status_register.hpp"
#include "spdlog/spdlog.h"
#include "tilemap.hpp"
#include <algorithm>
#include <cassert>

const int PPU::SCREEN_WIDTH = 160;
const int PPU::SCREEN_HEIGHT = 144;

PPU::~PPU() = default;

void PPU::step(int nbrTicks)
{
    _ticksSpentInCurrentMode += nbrTicks;

    bool areLYCAndLYEqual = _lcdStatusRegister->areLYCAndLYEqual();
    _lcdStatusRegister->setLYCompareFlag(areLYCAndLYEqual);

    if (areLYCAndLYEqual && _lcdStatusRegister->isLYCompareStatInterruptEnabled() && !_LYCInterruptRaisedDuringScanline)
    {
        _interruptManager->raiseInterrupt(InterruptType::LCD_STAT);
        _LYCInterruptRaisedDuringScanline = true;
    }

    if (_currentMode == OAM_ACCESS && _ticksSpentInCurrentMode >= OAM_ACCESS_TICKS)
    {
        _spritesToRender.clear();

        if (areSpritesEnabled())
        {
            _spritesToRender = getSpritesThatShouldBeRendered(_currentScanline);
        }

        setMode(VRAM_ACCESS);
        _pixelFifoRenderer.reset();
        _pixelFifoRenderer.setSpritesToRender(_spritesToRender);
    }
    else if (_currentMode == VRAM_ACCESS)
    {
        stepFifo(nbrTicks);
    }
    else if (_currentMode == HBLANK && _ticksSpentInCurrentMode >= (HBLANK_TICKS - _extraTicksSpentDrawingPixels))
    {
        _currentScanline++;

        _LYCInterruptRaisedDuringScanline = false;

        if (_currentScanline == SCREEN_HEIGHT)
        {
            if (_lcdStatusRegister->isVBLANKStatInterruptEnabled())
            {
                _interruptManager->raiseInterrupt(InterruptType::LCD_STAT);
            }

            _interruptManager->raiseInterrupt(InterruptType::VBLANK);
            setMode(VBLANK);
            swapFrameBuffers();
        }
        else
        {
            if (_lcdStatusRegister->isOAMStatInterruptEnabled())
            {
                _interruptManager->raiseInterrupt(InterruptType::LCD_STAT);
            }

            setMode(OAM_ACCESS);
        }
    }
    else if (_currentMode == VBLANK && _ticksSpentInCurrentMode >= VBLANK_TICKS)
    {
        _currentScanline++;

        _ticksSpentInCurrentMode = 0;
        if (_currentScanline > MAX_SCANLINE_VALUE)
        {
            if (_lcdStatusRegister->isOAMStatInterruptEnabled())
            {
                _interruptManager->raiseInterrupt(InterruptType::LCD_STAT);
            }

            setMode(OAM_ACCESS);
            _currentScanline = 0;
            _windowLineCounter = 0;
        }
        _LYCInterruptRaisedDuringScanline = false;
    }

    _lcdStatusRegister->setScanlineRegister(_currentScanline);
}

std::vector<Sprite*> PPU::getSpritesThatShouldBeRendered(int scanline)
{
    std::vector<Sprite*> spritesToRender = {};
    int nbrSpritesInScanline = 0;

    /*
     * In order to find which sprites to render, we need to check the coordinate of every sprite.
     * There's a maximum number of sprites that can be rendered per scanline.
     */
    for (int i = 0; i < NBR_SPRITES && nbrSpritesInScanline < MAX_NBR_SPRITES_PER_SCANLINE; ++i)
    {
        auto& sprite = _sprites[i];
        int spriteSz = spriteSize();
        int spriteStartVerticalPos = sprite->getYPositionOnScreen();
        int spriteEndVerticalPos = spriteStartVerticalPos + spriteSz;

        if (scanline >= spriteStartVerticalPos && scanline < spriteEndVerticalPos)
        {
            nbrSpritesInScanline++;
            /*
             * If the sprite is outside the screen bound then it's not going to be effectively rendered,
             * but it still counts as if it was rendered and should increment the number of sprites rendered.
             */
            if ((sprite->getXPositionOnScreen() + SingleTile::TILE_WIDTH) < 0 ||
                sprite->getXPositionOnScreen() >= SCREEN_WIDTH)
            {
                continue;
            }

            spritesToRender.push_back(sprite.get());
        }
    }

    if (nbrSpritesInScanline == MAX_NBR_SPRITES_PER_SCANLINE)
    {
        spdlog::debug("Rendering the maximum amount of {} sprites for scanline {}.", nbrSpritesInScanline, scanline);
    }

    /*
     * Sort sprites by priority:
     * - DMG: Lower X coordinate = higher priority (rendered on top). If X is equal, lower OAM index wins.
     * - CGB: Lower OAM index = higher priority (rendered on top).
     *
     * We sort in REVERSE priority order (lowest priority first) so that when we process sprites
     * in order, higher priority sprites overwrite lower priority ones.
     */
    if (_mmu.isColorModeSupported())
    {
        // CGB: Sort by OAM index descending (higher index = lower priority, processed first)
        std::sort(spritesToRender.begin(), spritesToRender.end(),
                  [](Sprite* a, Sprite* b) { return a->getId() > b->getId(); });
    }
    else
    {
        // DMG: Sort by X coordinate descending, then by OAM index descending
        // Higher X = lower priority, processed first
        std::sort(spritesToRender.begin(), spritesToRender.end(), [](Sprite* a, Sprite* b) {
            if (a->getXPositionOnScreen() != b->getXPositionOnScreen())
            {
                return a->getXPositionOnScreen() > b->getXPositionOnScreen();
            }
            return a->getId() > b->getId();
        });
    }

    return spritesToRender;
}

void PPU::swapFrameBuffers()
{
    _lastRenderedFrame = _temporaryFrame;
    _frameId++;
}

LCDStatusRegister* PPU::getLcdStatusRegister() const
{
    return _lcdStatusRegister.get();
}

PPU::PPU(MMU& mmu_, InterruptManager* interruptManager)
    : _mmu(mmu_), _interruptManager(interruptManager), _lcdStatusRegister(std::make_unique<LCDStatusRegister>()),
      _paletteBackground(_mmu, ADDR_PALETTE_BG), _paletteObj0(_mmu, ADDR_PALETTE_OBJ0),
      _paletteObj1(_mmu, ADDR_PALETTE_OBJ1), _pixelFifoRenderer(&_mmu, this)
{
    reset();

    for (unsigned int i = 0; i < _sprites.size(); ++i)
    {
        _sprites[i] = std::make_unique<Sprite>(_mmu.getOAM(), i);
    }
}

Tilemap PPU::getTileMap(int index)
{
    word tileMapAddr = (index == 0) ? ADDR_MAP_0 : ADDR_MAP_1;
    return Tilemap(&_mmu.getVRAM(), tileMapAddr);
}

bool PPU::isDisplayEnabled() const
{
    return utils::isNthBitSet(_lcdControl, 7);
}

int PPU::windowTileMapIndex() const
{
    return utils::isNthBitSet(_lcdControl, 6);
}

bool PPU::isWindowEnabled() const
{
    return utils::isNthBitSet(_lcdControl, 5);
}

sbyte PPU::backgroundAndWindowTileDataAreaIndex() const
{
    return static_cast<sbyte>(!utils::isNthBitSet(_lcdControl, 4));
}

int PPU::backgroundTileMapIndex() const
{
    return utils::isNthBitSet(_lcdControl, 3);
}

int PPU::spriteSize() const
{
    return utils::isNthBitSet(_lcdControl, 2) ? 16 : 8;
}

bool PPU::areSpritesEnabled() const
{
    return utils::isNthBitSet(_lcdControl, 1);
}

bool PPU::areBackgroundAndWindowEnabled() const
{
    return utils::isNthBitSet(_lcdControl, 0);
}

bool PPU::areBackgroundAndWindowDeprioritized() const
{
    return !utils::isNthBitSet(_lcdControl, 0);
}

void PPU::reset()
{
    _frameId = 0;
    _ticksSpentInCurrentMode = 0;
    setMode(OAM_ACCESS);
    _currentScanline = 0;
    _windowLineCounter = 0;
    _LYCInterruptRaisedDuringScanline = false;
    _temporaryFrame = RGBImage(SCREEN_HEIGHT, SCREEN_WIDTH);
    _lastRenderedFrame = RGBImage(SCREEN_HEIGHT, SCREEN_WIDTH);
}

void PPU::setMode(PPU::Mode value)
{
    _lcdStatusRegister->updateFlagMode(value);
    _currentMode = value;
    // TODO: Check if we should take into account modulo of ticks
    _ticksSpentInCurrentMode = 0;
}

byte PPU::getLcdControl() const
{
    return _lcdControl;
}

void PPU::setLcdControl(byte lcdControl)
{
    _lcdControl = lcdControl;
}

byte PPU::getScrollX() const
{
    return _scrollX;
}

void PPU::setScrollX(byte scrollX)
{
    _scrollX = scrollX;
}

byte PPU::getScrollY() const
{
    return _scrollY;
}

void PPU::setScrollY(byte scrollY)
{
    _scrollY = scrollY;
}

byte PPU::getWindowScrollX() const
{
    return _windowScrollX;
}

void PPU::setWindowScrollX(byte windowScrollX)
{
    _windowScrollX = windowScrollX;
}

byte PPU::getWindowScrollY() const
{
    return _windowScrollY;
}

void PPU::setWindowScrollY(byte windowScrollY)
{
    _windowScrollY = windowScrollY;
}

MMU& PPU::getMMU()
{
    return _mmu;
}

void PPU::stepFifo(int ticks)
{
    int ticksLeft = ticks;
    while (ticksLeft > 0)
    {
        _pixelFifoRenderer.step();

        if (_pixelFifoRenderer.getX() >= SCREEN_WIDTH)
        {
            // Increment window line counter if window was rendered on this scanline
            // This must happen before transitioning to HBLANK
            if (_pixelFifoRenderer.wasWindowTriggeredThisScanline())
            {
                _windowLineCounter++;
            }

            if (_lcdStatusRegister->isHBLANKStatInterruptEnabled())
            {
                _interruptManager->raiseInterrupt(InterruptType::LCD_STAT);
            }
            _extraTicksSpentDrawingPixels = _ticksSpentInCurrentMode - VRAM_ACCESS_TICKS;
            spdlog::debug("Mode 3 last for {} ticks, extending HBLANK by {} ticks", _ticksSpentInCurrentMode,
                         _extraTicksSpentDrawingPixels);
            setMode(HBLANK);
            return;
        }

        ticksLeft--;
    }
}

Palette* PPU::getPaletteBackground()
{
    return static_cast<Palette*>(&_paletteBackground);
}

Palette* PPU::getPaletteObj(int paletteId)
{
    if (paletteId == 0)
    {
        return static_cast<Palette*>(&_paletteObj0);
    }
    return static_cast<Palette*>(&_paletteObj1);
}

RGBImage& PPU::getTemporaryFrame()
{
    return _temporaryFrame;
}

PixelFifoRenderer& PPU::getPixelFifoRenderer()
{
    return _pixelFifoRenderer;
}

int PPU::getWindowLineCounter() const
{
    return _windowLineCounter;
}

void PPU::incrementWindowLineCounter()
{
    _windowLineCounter++;
}
