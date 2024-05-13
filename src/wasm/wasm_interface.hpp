#ifndef GROUBOY_WASM_INTERFACE_HPP
#define GROUBOY_WASM_INTERFACE_HPP

extern "C"
{
    struct EmulatorImpl;

    EmulatorImpl* init();
    void destroy(EmulatorImpl* emulator);
    bool loadROM(EmulatorImpl* emulator, const unsigned char* data, int length);
    void start(EmulatorImpl* emulator);
}

#endif // GROUBOY_WASM_INTERFACE_HPP
