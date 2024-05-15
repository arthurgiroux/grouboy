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
     */
    explicit InterruptManager(CPU* cpu);
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

    /**
     * Retrieves the interrupt enable flag.
     *
     * @return The interrupt enable flag value.
     */
    byte getInterruptEnableFlag() const;

    /**
     * Sets the interrupt enable flag.
     *
     * This function sets the interrupt enable flag to the specified value.
     *
     * @param interruptEnableFlag The value to set the interrupt enable flag to.
     */
    void setInterruptEnableFlag(byte interruptEnableFlag);

    /**
     * @brief Gets the interrupt flag.
     *
     * This function returns the interrupt flag value.
     *
     * @return The interrupt flag value.
     */
    byte getInterruptFlag() const;

    /**
     * Sets the interrupt flag.
     *
     * This function sets the interrupt flag to the specified value.
     *
     * @param interruptFlag The value to set the interrupt flag to.
     */
    void setInterruptFlag(byte interruptFlag);
  private:
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
    std::vector<std::unique_ptr<InterruptHandler>> _interruptHandlers;

    /**
     * This value tells us which interrupt type is enabled and should be treated
     * when they are triggered.
     * This value is a bitset where bits position represents a certain interrupt type.
     * If a bit is set, then the interrupt type is enabled.
     *
     * @see bitPositionInFlagForInterruptType for the bit mapping
     */
    byte _interruptEnableFlag = 0;

    /**
     * The interrupt flag tells us which interrupt was triggered and is pending.
     * This value is a bitset where bits position represents a certain interrupt type.
     * If a bit is set, then the interrupt type is pending.
     *
     * @see bitPositionInFlagForInterruptType for the bit mapping
     */
    byte _interruptFlag = 0;
};

#endif // GBEMULATOR_INTERRUPT_MANAGER_HPP
