#include <sstream>
#include "FeatureFpNotImplemented.h"

char const *Exceptions::FeatureFpNotImplemented::what() const noexcept {
	return this->_executor.c_str();
}

namespace {
	std::string build_what_string(const char* executor) {
		std::stringstream ss;
		ss << "The \"" << executor << "\" tried to execute a SIMD operation, but the FEATURE_FP has not been implemented yet";
		return ss.str();
	}
}

Exceptions::FeatureFpNotImplemented::FeatureFpNotImplemented(const char *executor) :
	std::runtime_error("FEATURE_FP has not been implemented yet"),
	_executor(build_what_string(executor)) {}
