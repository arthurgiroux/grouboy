#ifndef GROUBOY_WASM_INTERFACE_HPP
#define GROUBOY_WASM_INTERFACE_HPP

extern "C"
{
    void startEmulatorFromData(const unsigned char* data, int length);
}

#endif // GROUBOY_WASM_INTERFACE_HPP
