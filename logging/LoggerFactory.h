#pragma once
#include <memory>
#include "StdoutLogger.h"

namespace Logging {
    std::unique_ptr<LoggerBase> createLogger(const char* prefix);
}
