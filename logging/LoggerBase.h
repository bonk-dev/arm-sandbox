#pragma once
#include <ostream>

#include "LogLevel.h"

namespace Logging {
    class LoggerBase {
    private:
        LogLevel _level;
    public:
        explicit LoggerBase(LogLevel level);
        [[nodiscard]] LogLevel getLevel() const;

        virtual std::ostream& error();
        virtual std::ostream& info();
        virtual std::ostream& verbose();
        virtual ~LoggerBase() = default;
    };
}
