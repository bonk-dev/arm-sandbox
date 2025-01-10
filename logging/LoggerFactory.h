#pragma once
#include <memory>
#include "StdoutLogger.h"

namespace Logging {
    inline std::unique_ptr<LoggerBase> createLogger(const char* prefix, LogLevel level = LogLevel::Verbose) {
        return std::make_unique<StdoutLogger>(prefix, level);
    }
}
