#pragma once
#include "LoggerBase.h"

namespace Logging {
    class StdoutLogger : public LoggerBase {
    private:
        std::string _prefix;
    public:
        explicit StdoutLogger(const char* prefix, LogLevel level);
        std::ostream& error() override;
        std::ostream& info() override;
        std::ostream& verbose() override;
    };
}
