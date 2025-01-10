#pragma once
#include <ostream>

#include "LogLevel.h"

namespace Logging {
    class LoggerBase {
    private:
        LogLevel _level;
        std::string _prefix;
        std::ostream& _print(std::ostream& dest, LogLevel level) const;
    protected:
        virtual std::ostream& getStream() = 0;
    public:
        LoggerBase(const char* prefix, LogLevel level);
        [[nodiscard]] LogLevel getLevel() const;

        virtual std::ostream& error();
        virtual std::ostream& info();
        virtual std::ostream& verbose();
        virtual ~LoggerBase() = default;
    };
}
