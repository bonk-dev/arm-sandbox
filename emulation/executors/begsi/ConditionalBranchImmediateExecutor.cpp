#include "ConditionalBranchImmediateExecutor.h"
#include "../../nzcv.h"

void Executors::Begsi::ConditionalBranchImmediateExecutor::execute(
		const InstructionDefs::Begsi::ConditionalBranchImmediate &instruction, AArch64Cpu& cpu) {
	using Condition = InstructionDefs::Begsi::ConditionalBranchImmediate::Condition;

	bool isMet = false;
	const uint64_t nzcv = cpu.readNzcvRegister();
	switch (instruction.condition) {
		case Condition::Equal:
			isMet = nzcv::zero(nzcv);
			break;
		case Condition::NotEqual:
			isMet = nzcv::zero(nzcv) == 0;
			break;
		case Condition::CarrySet:
			isMet = nzcv::carry(nzcv);
			break;
		case Condition::CarryClear:
			isMet = nzcv::carry(nzcv) == 0;
			break;
		case Condition::Negative:
			isMet = nzcv::negative(nzcv);
			break;
		case Condition::NotNegative:
			isMet = nzcv::negative(nzcv) == 0;
			break;
		case Condition::OverflowSet:
			isMet = nzcv::overflow(nzcv);
			break;
		case Condition::OverflowClear:
			isMet = nzcv::overflow(nzcv) == 0;
			break;
		case Condition::Higher:
			isMet = nzcv::carry(nzcv) && nzcv::zero(nzcv) == 0;
			break;
		case Condition::LowerOrSame:
			isMet = nzcv::carry(nzcv) == 0 || nzcv::zero(nzcv);
			break;
		case Condition::GreaterOrEqual:
			isMet = nzcv::negative(nzcv) == nzcv::overflow(nzcv);
			break;
		case Condition::LessThan:
			isMet = nzcv::negative(nzcv) != nzcv::overflow(nzcv);
			break;
		case Condition::GreaterThan:
			isMet = nzcv::zero(nzcv) == 0 && nzcv::negative(nzcv) == nzcv::overflow(nzcv);
			break;
		case Condition::LessOrEqual:
			isMet = nzcv::zero(nzcv) || nzcv::negative(nzcv) != nzcv::overflow(nzcv);
			break;
		case Condition::Always:
			isMet = true;
			break;
		case Condition::Never:
			isMet = false;
			break;
		default:
			throw std::runtime_error("Invalid condition");
	}

	if (isMet) {
		uint64_t pc = cpu.getProgramCounter();
		this->branchTo(
				pc + instruction.immediate,
				Emulation::BranchType::DirectBranch,
				true,
				false,
				cpu);
	}
}


