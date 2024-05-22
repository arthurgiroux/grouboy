#ifndef GROUBOY_OAM_HPP
#define GROUBOY_OAM_HPP

#include "common/types.hpp"
#include "common/utils.hpp"
#include <vector>

class OAM
{
  public:
    OAM() = default;
    virtual ~OAM() = default;

    byte read(word addr) const;
    void write(word addr, byte value);

    const utils::AddressRange addressRange = utils::AddressRange(0xFE00, 0xFE9F);

  private:
    static const int MEM_SIZE = 160;
    /**
     * The underlying memory representation
     */
    std::vector<byte> _memory = std::vector<byte>(MEM_SIZE);
};

#endif // GROUBOY_OAM_HPP
