#pragma once
#include <stdexcept>

namespace Emulation::Exceptions {
    /**
     * @brief Thrown when a read/write occurs outside an allocated memory segment.
     */
    class EmulSegFault final : public std::runtime_error {
    public:
        explicit EmulSegFault();
    };
}
