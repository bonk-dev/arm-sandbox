#pragma once
#include "../AArch64Cpu.h"
#include "../../logging/LoggerBase.h"
#include "../../logging/LoggerFactory.h"

class EmulatedSymbol {
protected:
	std::unique_ptr<Logging::LoggerBase> _logger;
public:
	explicit EmulatedSymbol(const char* logPrefix) : _logger(Logging::createLogger(logPrefix)) {}
	virtual void execute(AArch64Cpu& cpu) {}
	virtual ~EmulatedSymbol() = default;
};
