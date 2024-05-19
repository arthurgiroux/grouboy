#ifndef MMU_H
#define MMU_H

#include <array>
#include <memory>
#include <set>
#include <stdexcept>

#include "cartridge.hpp"
#include "common/types.hpp"
#include "cpu/input_controller.hpp"
#include "graphics/palette/color_palette_memory_mapper.hpp"
#include "memory/mbc/memory_bank_controller.hpp"
#include "switchable_memory_bank.hpp"
#include "timer/timer.hpp"
#include "vram.hpp"

// Forward declaration
class APU;
class Timer;
class InputController;
class InterruptManager;
class LCDStatusRegister;
class PPU;

/**
 * The MMU class is responsible for managing the memory access and mapping.
 * It handles read and write operations to various memory regions such as ROM, RAM, I/O registers,
 * and other critical areas within the Game Boy architecture.
 */
class MMU
{
    /**
     * The internal memory mapping is the following:
            +----------------------+ <--- 0x0000
            | ROM Bank 00          |   16KiB
            | (Usually fixed)      |
            +----------------------+ <--- 0x4000
            | ROM Bank 01~NN       |   16KiB
            | (Switchable)         |
            +----------------------+ <--- 0x8000
            | Video RAM (VRAM)     |   8KiB
            | (Switchable in CGB)  |
            +----------------------+ <--- 0xA000
            | External RAM         |   8KiB
            | (Switchable if any)  |
            +----------------------+ <--- 0xC000
            | Work RAM (WRAM)      |  4KiB
            +----------------------+ <--- 0xD000
            | Work RAM (WRAM)      |  4KiB
            | (Switchable in CGB)  |
            +----------------------+ <--- 0xE000
            | Echo RAM             |
            | (Prohibited)         |
            +----------------------+ <--- 0xFE00
            | Object Attribute Mem |
            +----------------------+ <--- 0xFEA0
            | Not usable           |
            +----------------------+ <--- 0xFF00
            | I/O Registers        |
            +----------------------+ <--- 0xFF80
            | High RAM             |
            +----------------------+ <--- 0xFFFF
            | Interrupt Enable     |
            |      register        |
            +----------------------+
     */
  public:
    MMU();
    ~MMU() = default;

    /**
     * Read the value of the given memory location
     * @param addr The address of the location
     * @return The value in memory
     */
    byte read(const word& addr);

    /**
     * Read the value of two consecutive memory location
     * @param addr The address of the location
     * @return the 2-bytes value in memory
     */
    word readWord(const word& addr);

    /**
     * Write a given value to a memory location
     * @param addr The address of the location
     * @param value The byte value to write
     */
    void write(const word& addr, const byte& value);

    /**
     * Write a given two-byte value to two consecutive memory location
     * @param addr The address of the location
     * @param value  The 2-bytes value to write
     */
    void writeWord(const word& addr, const word& value);

    /**
     * Load a cartridge from a file.
     * @param filepath The location of the cartridge file.
     * @return true if loaded successfully, false otherwise
     */
    bool loadCartridgeFromFile(const std::string& filepath);

    /**
     * Load a cartridge from raw data.
     * @param data  the binary data to load
     * @return true if loaded successfully, false otherwise
     */
    bool loadCartridgeData(std::vector<byte> data);

    /**
     * @return true if the boot rom is active, false otherwise
     */
    bool isBootRomActive();

    /**
     * @return A pointer to the loaded cartridge
     */
    Cartridge* getCartridge();

    /**
     * Set the input controller that will be used for register mapping
     * @param controller the controller to use
     */
    void setInputController(InputController* controller);

    /**
     * Set the APU that will be used for register mapping
     * @param apu the APU to use
     */
    void setAPU(APU* apu);

    /**
     * Set the Timer that will be used for register mapping
     * @param timer The timer implementation to use
     */
    void setTimer(Timer* timer);

    /**
     * Set the Interrupt Manager that will be used to get/set the interrupt flags.
     * @param interruptManager The manager to use
     */
    void setInterruptManager(InterruptManager* interruptManager);

    /**
     * Reset the state of the MMU
     */
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

    /**
     * Returns if emulator should render color, i.e. it's either in the bootrom or
     * the loaded rom supports color mode.
     * @return true if color is supported, false otherwise
     */
    bool isColorModeSupported();

    VRAM& getVRAM();

    ColorPaletteMemoryMapper& getColorPaletteMemoryMapperBackground();
    ColorPaletteMemoryMapper& getColorPaletteMemoryMapperObj();

    /**
     * Set the LCD Status register that will be used to get/set info about the LCD.
     * @param lcdStatusRegister A pointer to the register
     */
    void setLcdStatusRegister(LCDStatusRegister* lcdStatusRegister);

    /**
     * Set the PPU to use to set/get the graphical registers
     * @param ppu A pointer to the PPU implementation
     */
    void setPPU(PPU* ppu);

    /**
     * The total size of the memory in bytes
     */
    static const size_t MEMORY_SIZE_IN_BYTES = 65536;

    /**
     * The boot rom that will be loaded when the emulator is booted
     */
    static const std::array<byte, 2304> BOOTROM;

    /**
     * Exception class for invalid memory access
     */
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
        STAT = 0xFF41,
    };

    /**
     * bitmask for unused bits for the mapped hardware IO
     */
    static const std::map<word, byte> mappedIOMask;

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

    /**
     * Get the memory representation of the joypad state.
     * @return a bitmasked byte representing the state of the joypad
     */
    byte getJoypadMemoryRepresentation();

    /**
     * Perform a DMA transfer from the given source to the DMA destination
     * @param sourceAddr The source address for the DMA
     */
    void performDMATransfer(word sourceAddr);

    /**
     * If the emulator is currently in the bootrom
     */
    bool isInBootrom = true;

    /**
     * The internal representation of the memory
     */
    std::vector<byte> memory = std::vector<byte>(MEMORY_SIZE_IN_BYTES);

    /**
     * The loaded cartridge
     */
    std::unique_ptr<Cartridge> cartridge = nullptr;

    /**
     * The memory bank that will be used for the cartridge mapping
     */
    std::unique_ptr<MemoryBankController> memoryBankController = nullptr;

    /**
     * The control to be used for input mapping
     */
    InputController* inputController = nullptr;

    /**
     * External RAM address range
     */
    const utils::AddressRange externalRamAddr = utils::AddressRange(0xA000, 0xBFFF);

    /**
     * The end address of the 1st ROM bank
     */
    static constexpr word ROM_BANK_1_END_ADDR = 0x8000;

    /**
     * The address of unmapped flag in the boot room
     */
    static constexpr word BOOT_ROM_UNMAPPED_FLAG_ADDR = 0xFF50;

    /**
     * The address of the memory location where joypad state will be mapped
     */
    static constexpr word JOYPAD_MAP_ADDR = 0xFF00;

    /**
     * The address of the DMA control location
     */
    static constexpr word DMA_TRANSFER_ADDR = 0xFF46;

    /**
     * The length in bytes of the DMA transfer
     */
    static const int DMA_TRANSFER_LENGTH = 160;

    /**
     * The target address of a DMA transfer
     */
    static constexpr word DMA_TRANSFER_TARGET_ADDR = 0xFE00;

    /**
     * The address in memory where the cartridge header will be mapped
     */
    const utils::AddressRange cartridgeHeaderAddr = utils::AddressRange(0x0100, 0x014F);

    /**
     * The address where the value of the Divider Register is stored.
     */
    static constexpr word TIMER_DIV_ADDR = 0xFF04;

    /**
     * The address where the value of the Timer Counter is stored.
     */
    static constexpr word TIMER_COUNTER_ADDR = 0xFF05;

    /**
     * The address where the value of the Timer Modulo is stored.
     */
    static constexpr word TIMER_MODULO_ADDR = 0xFF06;

    /**
     * The address where the parameters of the Timer Counter are stored.
     */
    static constexpr word TIMER_CONTROL_ADDR = 0xFF07;

    /**
     * Set bitmask value depending on the state of a button.
     *
     * @param button the button to check
     * @param bitPosition the bitmask position to set
     * @param value The output value to modify
     */
    void setNthBitIfButtonIsReleased(InputController::Button button, int bitPosition, int& value);

    /**
     * The APU to use for audio register mapping
     */
    APU* _apu = nullptr;

    /**
     * The timer to use for timer register mapping
     */
    Timer* _timer = nullptr;

    /**
     * The address of the VRAM bank id register
     */
    static constexpr word VRAM_BANK_ID_ADDR = 0xFF4F;

    /**
     * The VRAM
     */
    VRAM vram;

    /**
     * The address of the WRAM bank id register
     */
    static constexpr word WRAM_BANK_ID_ADDR = 0xFF70;

    /**
     * The address of the WRAM mapping
     */
    const utils::AddressRange wramAddressRange = utils::AddressRange(0xD000, 0xDFFF);

    /**
     * The memory bank to use for the WRAM
     */
    SwitchableMemoryBank<7, 4_KiB> wramMemoryBank;

    static constexpr word COLOR_PALETTE_SPECS_BACKGROUND_ADDR = 0xFF68;
    static constexpr word COLOR_PALETTE_DATA_BACKGROUND_ADDR = 0xFF69;

    static constexpr word COLOR_PALETTE_SPECS_OBJECTS_ADDR = 0xFF6A;
    static constexpr word COLOR_PALETTE_DATA_OBJECTS_ADDR = 0xFF6B;

    ColorPaletteMemoryMapper colorPaletteMemoryMapperBackground;
    ColorPaletteMemoryMapper colorPaletteMemoryMapperObjects;

    /**
     * The address in memory of the "enable interrupt" value.
     */
    static const int INTERRUPT_ENABLE_ADDR = 0xFFFF;

    /**
     * The address in memory of the "interrupt flag".
     */
    static const int INTERRUPT_FLAG_ADDR = 0xFF0F;

    /**
     * The interrupt manager to use to get/set the interrupts flags value.
     */
    InterruptManager* _interruptManager = nullptr;

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

    /**
     * The LCD Status register to use to get/set information about the LCD Status
     */
    LCDStatusRegister* _lcdStatusRegister = nullptr;

    /**
     * The PPU to use to set/get the graphical registers
     */
    PPU* _ppu = nullptr;

    /**
     * The address of the LCD control register.
     */
    static constexpr word ADDR_LCD_PPU_CONTROL = 0xFF40;

    /**
     * The address of the scroll-y register.
     */
    static constexpr word ADDR_SCROLL_Y = 0xFF42;

    /**
     * The address of the scroll-x register.
     */
    static constexpr word ADDR_SCROLL_X = 0xFF43;

    /**
     * The address of the scroll-y window register.
     */
    static constexpr word WINDOW_ADDR_SCROLL_Y = 0xFF4A;

    /**
     * The address of the scroll-x window register.
     */
    static constexpr word WINDOW_ADDR_SCROLL_X = 0xFF4B;
};

#endif
