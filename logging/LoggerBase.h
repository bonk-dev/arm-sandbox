#pragma once
#include <ostream>

#include "LogLevel.h"

namespace Logging {
    class LoggerBase {
    private:
        LogLevel _level;
        std::string _prefix;
        std::ostream& _print(std::ostream& dest, LogLevel level, bool printPrefix) const;
    protected:
        virtual std::ostream& getStream() = 0;
    public:
        LoggerBase(const char* prefix, LogLevel level);

		[[nodiscard]] LogLevel getLevel() const;
		void setLevel(LogLevel level);

		virtual std::ostream& error();
		virtual std::ostream& info();
		virtual std::ostream& verbose();

        virtual std::ostream& error(bool printPrefix);
        virtual std::ostream& info(bool printPrefix);
        virtual std::ostream& verbose(bool printPrefix);

        virtual ~LoggerBase() = default;
    };
}
