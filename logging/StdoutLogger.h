#pragma once
#include "LoggerBase.h"

namespace Logging {
    class StdoutLogger : public LoggerBase {
    protected:
        std::ostream& getStream() override;
    public:
        explicit StdoutLogger(const char* prefix, LogLevel level);
    };
}
