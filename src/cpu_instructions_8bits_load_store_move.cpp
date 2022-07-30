#include "cpu.hpp"

using namespace utils;

void CPU::loadValueToMemoryAtAddr(byte addrMsb, byte addrLsb, byte value)
{
    mmu.write(createAddrFromHighAndLowBytes(addrMsb, addrLsb), value);
    lastInstructionTicks = 2;
}

void CPU::load8BitsImmediateValueAtMemoryAddress(byte addrMsb, byte addrLsb)
{
    byte value = mmu.read(pc);
    pc++;
    mmu.write(createAddrFromHighAndLowBytes(addrMsb, addrLsb), value);
    lastInstructionTicks = 3;
}

void CPU::loadImmediateValueInRegister(byte& reg)
{
    reg = mmu.read(pc);
    pc++;
    lastInstructionTicks = 2;
}

void CPU::loadValueToMemoryAndIncrementAddr(byte& addrMsb, byte& addrLsb, byte value)
{
    mmu.write(createAddrFromHighAndLowBytes(addrMsb, addrLsb), value);
    increment16BitsValueStoredIn8BitsValues(addrMsb, addrLsb);
    lastInstructionTicks = 2;
}

void CPU::loadValueFromMemoryAndIncrementAddr(byte& reg, byte& addrMsb, byte& addrLsb)
{
    reg = mmu.read(createAddrFromHighAndLowBytes(addrMsb, addrLsb));
    increment16BitsValueStoredIn8BitsValues(addrMsb, addrLsb);
    lastInstructionTicks = 2;
}

void CPU::loadValueFromMemoryAndDecreaseAddr(byte& reg, byte& addrMsb, byte& addrLsb)
{
    reg = mmu.read(createAddrFromHighAndLowBytes(addrMsb, addrLsb));
    decrement16BitsValueStoredIn8BitsValues(addrMsb, addrLsb);
    lastInstructionTicks = 2;
}

void CPU::loadValueToMemoryAndDecreaseAddr(byte& addrMsb, byte& addrLsb, byte value)
{
    mmu.write(createAddrFromHighAndLowBytes(addrMsb, addrLsb), value);
    decrement16BitsValueStoredIn8BitsValues(addrMsb, addrLsb);
    lastInstructionTicks = 2;
}

void CPU::loadValueFromMemoryInto8BitsRegister(byte& reg, byte addrMsb, byte addrLsb)
{
    reg = mmu.read(createAddrFromHighAndLowBytes(addrMsb, addrLsb));
    lastInstructionTicks = 2;
}

void CPU::load8BitsValueInRegister(byte& reg, byte value)
{
    reg = value;
    lastInstructionTicks = 1;
}

void CPU::load8BitsRegisterAtImmediateAddress(byte reg)
{
    mmu.write(mmu.readWord(pc), reg);
    pc += 2;
    lastInstructionTicks = 4;
}

void CPU::add8BitsValueTo8BitsRegister(byte& reg, byte value)
{
    unsetFlag(CpuFlags::SUBSTRACTION);
    uint16_t result = reg + value;
    setCarryFlag(result > 0xFF);
    setHalfCarryFlag(reg <= 0x0F && result > 0x0F);
    reg = static_cast<byte>(result);
    changeZeroValueFlag(reg);
    lastInstructionTicks = 1;
}

