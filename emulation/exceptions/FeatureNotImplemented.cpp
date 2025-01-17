#include <sstream>
#include "FeatureNotImplemented.h"

namespace {
	std::string build_what_string(const char* feature, const char* executor) {
		std::stringstream ss;
		ss << "The \"" << executor << "\" tried to use an instruction, which comes from a unimplemented CPU feature: FEATURE_" << feature;
		return ss.str();
	}
}

Exceptions::FeatureNotImplemented::FeatureNotImplemented(const char* feature, const char *executor) :
	std::runtime_error(build_what_string(feature, executor).c_str()) {}
