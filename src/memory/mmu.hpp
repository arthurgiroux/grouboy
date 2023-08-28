#ifndef MMU_H
#define MMU_H

#include <array>
#include <memory>
#include <set>
#include <stdexcept>

#include "cartridge.hpp"
#include "common/types.hpp"
#include "cpu/input_controller.hpp"
#include "memory/mbc/memory_bank_controller.hpp"
#include "timer/timer.hpp"

// Forward declaration
class APU;
class Timer;
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
    void setAPU(APU* apu);
    void setTimer(Timer* timer);
    void reset();

    /**
     * Serialize the current state of the cartridge RAM.
     *
     * @return  a binary representation of the cartridge RAM that can be restored later
     */
    std::vector<byte> serializeCartridgeRAM();

    /**
     * Load a state of the cartridge RAM that was previously serialized using serializeCartridgeRAM().
     *
     * @param data  the serialized data to load.
     */
    bool unserializeCartridgeRAM(const std::vector<byte>& data);

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
    /**
     * Addresses of Hardware IO that are mapped by the MMU
     */
    enum HardwareIOAddr : word
    {
        P1 = 0xFF00,
        SC = 0xFF02,
        IF = 0xFF0F,
        STAT = 0xFF41,
        BOOT_ROOM_LOCK = 0xFF50,
    };

    /**
     * bitmask for unused bits for the mapped hardware IO
     */
    static const std::map<word, int> mappedIOMask;

    /**
     * List of non-consecutive unmapped IO addresses
     */
    static const std::set<word> unmappedIOAddrs;

    /**
     * Range of unmapped IO addresses
     */
    static const utils::AddressRange unmappedIOAddrRange;

    /**
     * Range of APU register addresses
     */
    static const utils::AddressRange apuRegisterRange;

    byte getJoypadMemoryRepresentation();

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

    /**
     * The address where the value of the Divider Register is stored.
     */
    static const int TIMER_DIV_ADDR = 0xFF04;

    /**
     * The address where the value of the Timer Counter is stored.
     */
    static const int TIMER_COUNTER_ADDR = 0xFF05;

    /**
     * The address where the value of the Timer Modulo is stored.
     */
    static const int TIMER_MODULO_ADDR = 0xFF06;

    /**
     * The address where the parameters of the Timer Counter are stored.
     */
    static const int TIMER_CONTROL_ADDR = 0xFF07;

    void setNthBitIfButtonIsReleased(InputController::Button button, int bitPosition, int& value);

    APU* _apu = nullptr;
    Timer* _timer = nullptr;
};

#endif
