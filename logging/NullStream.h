#pragma once
#include <ostream>

namespace Logging {
	/**
	 * @brief A stream that accepts data and discards it
	 */
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

	/**
	 * @brief Get a shared instance of NullStream
	 * @return The shared NullStream instance
	 */
    NullStream& getNullStream();
}
