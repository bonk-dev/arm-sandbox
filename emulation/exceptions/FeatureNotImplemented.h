#pragma once

#include <stdexcept>

namespace Exceptions {
	class FeatureNotImplemented : public std::runtime_error {
	public:
		explicit FeatureNotImplemented(const char* feature, const char *executor);
	};
}
