#pragma once

#include "../../../disassembly/instructions/reserved/ReservedCall.h"
#include "../ExecutorBaseTemplated.h"
#include "../../libraries/Mapper.h"

namespace Executors::Reserved {
	class ReservedCallExecutor : public ExecutorBaseT<InstructionDefs::Reserved::ReservedCall> {
	private:
		std::shared_ptr<Emulation::Libraries::Mapper> _mapper;
	public:
		explicit ReservedCallExecutor(const std::shared_ptr<AArch64Cpu> &cpu);
		ReservedCallExecutor(const std::shared_ptr<AArch64Cpu> &cpu, const std::shared_ptr<Emulation::Libraries::Mapper>& mapper);
		void execute(const InstructionDefs::Reserved::ReservedCall& instruction) override;
	};
}
