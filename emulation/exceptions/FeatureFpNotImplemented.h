#pragma once

#include <stdexcept>

namespace Exceptions {
	class FeatureFpNotImplemented : public std::runtime_error {
	private:
		std::string _executor;
	public:
		explicit FeatureFpNotImplemented(const char *executor);

		[[nodiscard]] char const *what() const noexcept override;
	};
}
