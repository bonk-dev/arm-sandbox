#pragma once

#include "ExecutorBase.h"
#include "../../disassembly/disassembly.h"
#include "../../logging/LoggerBase.h"
#include "../../logging/LoggerFactory.h"

template <class InstructionDetailsT>
class ExecutorBaseT : public ExecutorBase {
protected:
	std::unique_ptr<Logging::LoggerBase> _logger;
public:
	explicit ExecutorBaseT(const char* name): _logger(Logging::createLogger(name)) {}

	virtual void execute(const InstructionDetailsT& details, AArch64Cpu& cpu) = 0;
	void decodeAndLog(uint32_t encoded) override {
		const auto details = InstructionDetailsT(encoded);

		// Log disassembly
		_logger->info(false) <<  disassembly::to_pretty_string(details) << std::endl;
	}
	void decodeAndExecute(uint32_t encoded, AArch64Cpu& cpu) override {
		const auto details = InstructionDetailsT(encoded);

		// Log disassembly
		_logger->info(false) <<  disassembly::to_pretty_string(details) << std::endl;

		this->execute(details, cpu);
	}
};
