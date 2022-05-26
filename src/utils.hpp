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
}

#endif //GBEMULATOR_UTILS_HPP
