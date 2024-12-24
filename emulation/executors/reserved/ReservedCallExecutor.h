#pragma once

#include "../../../disassembly/instructions/reserved/ReservedCall.h"
#include "../ExecutorBaseTemplated.h"

namespace Executors::Reserved {
	class ReservedCallExecutor : public ExecutorBaseT<InstructionDefs::Reserved::ReservedCall> {
	public:
		explicit ReservedCallExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
		void execute(const InstructionDefs::Reserved::ReservedCall& instruction) override;
	};
}
