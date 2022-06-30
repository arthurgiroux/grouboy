#ifndef GBEMULATOR_UTILS_HPP
#define GBEMULATOR_UTILS_HPP

#include "types.hpp"

namespace utils {

    /**
     * Create a 16 bits address from an MSB and an LSB.
     *
     * @param msb   the high byte
     * @param lsb   the low byte
     * @return      a 16 bits unsigned address
     */
    static uint16_t createAddrFromHighAndLowBytes(byte msb, byte lsb) {
        return (msb << 8u) | lsb;
    }

    /**
     * Format a string using a "printf"-style format.
     * @param fmt   The string to format
     * @param args  The value used for formatting
     * @return      The formatting string
     */
    template<typename... Args>
    static std::string string_format(const char* fmt, Args... args)
    {
        size_t size = snprintf(nullptr, 0, fmt, args...);
        std::string buf;
        buf.reserve(size + 1);
        buf.resize(size);
        snprintf(&buf[0], size + 1, fmt, args...);
        return buf;
    }
}

#endif //GBEMULATOR_UTILS_HPP
