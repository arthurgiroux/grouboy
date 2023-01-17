#ifndef MMU_H
#define MMU_H

#include <array>
#include <memory>
#include <stdexcept>

#include "cartridge.hpp"
#include "common/types.hpp"
#include "cpu/input_controller.hpp"
#include "memory/mbc/memory_bank_controller.hpp"
#include "timer/timer.hpp"

// Forward declaration
class InputController;

/***********************************
            MEMORY LAYOUT

 [ BIOS | ROM BANK 0 | ROM BANK 1 | GPU VRAM | Ext. RAM | Working RAM | Sprites info | I/O | ZRAM ]
 0     256b   8k    16k    24k   32k        40k        48k           56k                         64k


 ***********************************/

class MMU
{
  public:
    MMU();
    ~MMU() = default;

    byte read(const word& addr);
    word readWord(const word& addr);
    void write(const word& addr, const byte& value);
    void writeWord(const word& addr, const word& value);
    bool loadCartridge(const std::string& filepath);
    bool isBootRomActive();
    Cartridge* getCartridge();
    void setInputController(InputController* controller);
    void reset();

    static const size_t MEMORY_SIZE_IN_BYTES = 65536;
    static const std::array<byte, 256> BIOS;

    class InvalidMemoryAccessException : public std::exception
    {
      public:
        const char* what() const noexcept override
        {
            return "Invalid memory access.";
        }
    };

  private:
    void performDMATransfer(word sourceAddr);
    std::array<byte, MEMORY_SIZE_IN_BYTES> memory{};
    std::unique_ptr<Cartridge> cartridge = nullptr;
    std::unique_ptr<MemoryBankController> memoryBankController = nullptr;
    static const int EXTERNAL_RAM_START_ADDR = 0xA000;
    static const int EXTERNAL_RAM_END_ADDR = 0xC000;
    static const int ROM_BANK_1_END_ADDR = 0x8000;
    static const int BOOT_ROM_UNMAPPED_FLAG_ADDR = 0xFF50;
    InputController* inputController = nullptr;
    static const int JOYPAD_MAP_ADDR = 0xFF00;
    static const int DMA_TRANSFER_ADDR = 0xFF46;
    static const int DMA_TRANSFER_LENGTH = 160;
    static const int DMA_TRANSFER_TARGET_ADDR = 0xFE00;

    byte getJoypadMemoryRepresentation();
    void setNthBitIfButtonIsReleased(InputController::Button button, int bitPosition, int& value);

    // The Timer class is allowed to directly access the internal memory representation.
    // This is so that they can circumvent the check when writing to the timer divider register.
    // TODO: Expose an "unsafe write" function to better encapsulate this.
    friend class Timer;
};

#endif
