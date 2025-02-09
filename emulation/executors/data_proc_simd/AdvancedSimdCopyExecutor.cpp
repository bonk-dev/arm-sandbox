#include "AdvancedSimdCopyExecutor.h"

void Executors::DataProcReg::AdvancedSimdCopyExecutor::execute(
		const InstructionDefs::DataProcSimd::AdvancedSimdCopy &details, AArch64Cpu &cpu) {
	auto& neon = cpu.getNeon();

	switch (details.copyType) {
		case InstructionDefs::DataProcSimd::AdvancedSimdCopy::CopyType::DuplicateElement:
		case InstructionDefs::DataProcSimd::AdvancedSimdCopy::CopyType::DuplicateGeneral:
		{
			const unsigned int eSize = details.getElementSize();
			const uint64_t sourceElement = neon.readElement(
					details.sourceRegIndex,
					details.getSourceElementIndex(),
					eSize,
					details.getSourceElementIndexDataSize());
			for (size_t i = 0; i < details.getElementCount(); ++i) {
				neon.writeElement(
						details.destinationRegIndex,
						i,
						eSize,
						sourceElement,
						details.getSourceElementIndexDataSize());
			}
			break;
		}
		case InstructionDefs::DataProcSimd::AdvancedSimdCopy::CopyType::SignedMove:
		case InstructionDefs::DataProcSimd::AdvancedSimdCopy::CopyType::UnsignedMove:
		case InstructionDefs::DataProcSimd::AdvancedSimdCopy::CopyType::InsertGeneral:
		case InstructionDefs::DataProcSimd::AdvancedSimdCopy::CopyType::InsertElement:
			// TODO: Copy types not implemented
			throw std::runtime_error("TODO: Copy type not implemented");
		default:
			throw std::runtime_error("Invalid Advanced SIMD copy operation");
	}
}
