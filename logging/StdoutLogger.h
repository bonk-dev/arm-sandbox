#pragma once
#include "LoggerBase.h"

namespace Logging {
	/**
	 * @brief A logger implementation which outputs messages to stdout (and stderr)
	 */
    class StdoutLogger : public LoggerBase {
    protected:
        std::ostream& getStream() override;
    public:
        explicit StdoutLogger(const char* prefix, LogLevel level);
    };
}
