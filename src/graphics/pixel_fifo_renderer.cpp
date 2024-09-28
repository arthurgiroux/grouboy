#include "pixel_fifo_renderer.hpp"
#include "ppu.hpp"

PixelFifoRenderer::PixelFifoRenderer(MMU* mmu, PPU* ppu)
    : _mmu(mmu), _ppu(ppu), _bgWindowPixelFetcher(&mmu->getVRAM(), ppu, _backgroundWindowFIFO)
{
}

void PixelFifoRenderer::step()
{
    _bgWindowPixelFetcher.step();

    if (_backgroundWindowFIFO.size() >= 8)
    {
        // TODO: mix pixels
        Pixel pixel = _backgroundWindowFIFO.pop();
        Palette* palette = _ppu->getPaletteBackground();
        if (_mmu->isColorModeSupported())
        {
            palette = &_mmu->getColorPaletteMemoryMapperBackground().getColorPalette(pixel.getPaletteId());
        }
        _ppu->getTemporaryFrame().setPixel(_x, _ppu->getCurrentScanline(), palette->getColorForId(pixel.getColorId()));
        _x++;
    }
}
int PixelFifoRenderer::getX() const
{
    return _x;
}

void PixelFifoRenderer::reset()
{
    _backgroundWindowFIFO.clear();
    _spritesFIFO.clear();
    _x = 0;
    _bgWindowPixelFetcher.reset();
}
