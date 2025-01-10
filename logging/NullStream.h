#pragma once
#include <ostream>

namespace Logging {
    class NullStream final : public std::ostream {
        class NullBuffer final : public std::streambuf {
            int_type overflow(int_type c) noexcept override {
                return c;
            }
        } nullBuffer;

    public:
        NullStream() : std::ostream(&nullBuffer) {}
        NullStream(const NullStream&) = delete;
        NullStream& operator=(const NullStream&) = delete;
    };

    NullStream& getNullStream();
}
