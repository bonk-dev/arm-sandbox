#include "ConditionalBranchImmediateExecutor.h"
#include "../../nzcv.h"

void Executors::Begsi::ConditionalBranchImmediateExecutor::execute(
		const InstructionDefs::Begsi::ConditionalBranchImmediate &instruction, AArch64Cpu& cpu) {
	using Condition = InstructionDefs::Begsi::ConditionalBranchImmediate::Condition;

	bool isMet = false;
	const uint64_t nzcv = cpu.readNzcvRegister();
	switch (instruction.condition) {
		case Condition::Equal:
			isMet = NZCV::Zero(nzcv);
			break;
		case Condition::NotEqual:
			isMet = NZCV::Zero(nzcv) == 0;
			break;
		case Condition::CarrySet:
			isMet = NZCV::Carry(nzcv);
			break;
		case Condition::CarryClear:
			isMet = NZCV::Carry(nzcv) == 0;
			break;
		case Condition::Negative:
			isMet = NZCV::Negative(nzcv);
			break;
		case Condition::NotNegative:
			isMet = NZCV::Negative(nzcv) == 0;
			break;
		case Condition::OverflowSet:
			isMet = NZCV::Overflow(nzcv);
			break;
		case Condition::OverflowClear:
			isMet = NZCV::Overflow(nzcv) == 0;
			break;
		case Condition::Higher:
			isMet = NZCV::Carry(nzcv) && NZCV::Zero(nzcv) == 0;
			break;
		case Condition::LowerOrSame:
			isMet = NZCV::Carry(nzcv) == 0 || NZCV::Zero(nzcv);
			break;
		case Condition::GreaterOrEqual:
			isMet = NZCV::Negative(nzcv) == NZCV::Overflow(nzcv);
			break;
		case Condition::LessThan:
			isMet = NZCV::Negative(nzcv) != NZCV::Overflow(nzcv);
			break;
		case Condition::GreaterThan:
			isMet = NZCV::Zero(nzcv) == 0 && NZCV::Negative(nzcv) == NZCV::Overflow(nzcv);
			break;
		case Condition::LessOrEqual:
			isMet = NZCV::Zero(nzcv) || NZCV::Negative(nzcv) != NZCV::Overflow(nzcv);
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


