#ifndef GBEMULATOR_INTERRUPT_MANAGER_HPP
#define GBEMULATOR_INTERRUPT_MANAGER_HPP

#include "cpu/interrupt_manager.hpp"
#include "memory/mmu.hpp"

// Forward declaration
class CPU;
class InterruptHandler;

/**
 * The possible type of interrupts that can be triggered by the system.
 */
enum class InterruptType
{
    /**
     * Requested everytime the GameBoy enters Vertical Blank (~60Hz)
     */
    VBLANK,
    /**
     * LCD Status interrupt, can be used for various events of the PPU.
     */
    LCD_STAT,
    /**
     * Timer interrupt, can be programmed using the Timer abstraction to be
     * raised at a specific frequency.
     */
    TIMER,
    /**
     * Serial interrupt, will be raised by the serial abstraction when a new
     * value is available on the serial line.
     */
    SERIAL,
    /**
     * Joypad interrupt, will be raised when a button is pressed.
     */
    JOYPAD
};

/**
 * A generic Manager class for system interrupts.
 * This class is responsible for enabling/disabling interrupts,
 * Raising/checking/clearing pending interrupts,
 * and calling interrupt handlers to handle pending interrupts.
 */
class InterruptManager
{
  public:
    /**
     * Create a new interrupt manager.
     * @param cpu   The CPU instance to use for calling interrupt routines.
     * @param mmu   The MMU instance to use for memory access.
     */
    explicit InterruptManager(CPU* cpu, MMU* mmu);
    ~InterruptManager() = default;

    /**
     * Is there any interrupt that is enabled.
     * @return true if any interrupt is enabled, false otherwise.
     */
    bool isAnyInterruptEnabled();

    /**
     * Is a specific interrupt type enabled.
     * @param type the interrupt type to check.
     * @return true if the interrupt is enabled, false otherwise.
     */
    bool isInterruptEnabled(InterruptType type);

    /**
     * Is there any interrupt that is pending.
     * @return true if any interrupt is pending, false otherwise.
     */
    bool isAnyInterruptPending();

    /**
     * Is a specific interrupt type pending.
     * @param type the interrupt type to check.
     * @return true if the interrupt is pending, false otherwise.
     */
    bool isInterruptPending(InterruptType type);

    /**
     * Raise a specific interrupt.
     * @param type The type of the interrupt to raise.
     */
    void raiseInterrupt(InterruptType type);

    /**
     * Clear a specific interrupt.
     * @param type The type of the interrupt to clear.
     */
    void clearInterrupt(InterruptType type);

    /**
     * Handle the first (ordered by priority) enabled and pending interrupts.
     * Handling an interrupt involves clearing it and calling the interrupt routine.
     * @return Return true if any interrupt was handled, false otherwise.
     */
    bool handleInterrupts();

  private:
    /**
     * The address in memory of the "enable interrupt" value.
     * This value tells us which interrupt type is enabled and should be treated
     * when they are triggered.
     * This value is a bitflag where bits position represents a certain interrupt type.
     * If a bit is set, then the interrupt type is enabled.
     *
     * @see bitPositionInFlagForInterruptType for the bit mapping
     */
    static const int INTERRUPT_ENABLE_ADDR = 0xFFFF;

    /**
     * The address in memory of the "interrupt flag".
     * The interrupt flag tells us which interrupt was triggered and is pending.
     * This value is a bitflag where bits position represents a certain interrupt type.
     * If a bit is set, then the interrupt type is pending.
     *
     * @see bitPositionInFlagForInterruptType for the bit mapping
     */
    static const int INTERRUPT_FLAG_ADDR = 0xFF0F;

    /**
     * The mapping between an interrupt type and the bit position for the
     * different bitflags.
     * Can be used to access a specific value in the "interrupt flag" and "interrupt enable" value.
     */
    std::map<InterruptType, int> bitPositionInFlagForInterruptType = {
        {InterruptType::VBLANK, 0}, {InterruptType::LCD_STAT, 1}, {InterruptType::TIMER, 2},
        {InterruptType::SERIAL, 3}, {InterruptType::JOYPAD, 4},
    };

    /**
     * A list of interrupts handlers to be used to handle the different interrupts type.
     * The order of the handlers in the list represents their priorities:
     * _interruptHandlers[0] has a higher priority than _interruptHandlers[1]
     */
    std::vector<std::unique_ptr<InterruptHandler>> _interruptHandlers = {};

    /**
     * The MMU to use for memory operations.
     */
    MMU* _mmu = nullptr;
};

#endif // GBEMULATOR_INTERRUPT_MANAGER_HPP
