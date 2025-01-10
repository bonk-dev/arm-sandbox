#include "Mapper.h"
#include "../../disassembly/instructions/reserved/ReservedCall.h"
#include <stdexcept>

#include "SymbolNotImplemented.h"
#include "../../disassembly/instructions/begsi/UnconditionalBranchImmediate.h"
#include "../../disassembly/instructions/begsi/UnconditionalBranchRegister.h"

Emulation::Libraries::Mapper::Mapper() :
	_logger(Logging::createLogger("Mapper")),
	_nextIndex(0),
	_indexSymbols(std::make_unique<std::map<unsigned int, std::shared_ptr<library_impl_symbol_t>>>()),
	_implementations(std::make_unique<std::map<const char*, std::shared_ptr<library_impl_symbol_t>, str_cmp>>()) {}

void Emulation::Libraries::Mapper::registerLibraryImplementation(
    const char *symbolName,
    library_impl_exec_t implementation) {
    auto sym = std::make_shared<library_impl_symbol_t>(library_impl_symbol_t{
        std::move(implementation),
        _nextIndex++
    });
    this->_implementations->emplace(symbolName, sym);
    this->_indexSymbols->emplace(sym->index, sym);
}

virtual_address_t Emulation::Libraries::Mapper::mapLibraryImplementation(const char *symbolName,
    CpuVirtualMemory &memory) {
    // A64:
	// UDF #libraryCall, #symbolIndex
	// RET

	if (!this->_linkingTableAddress.has_value()) {
		throw std::runtime_error("Linking table has not been allocated yet");
	}

	auto symIter = this->_implementations->find(symbolName);
	if (symIter == this->_implementations->end()) {
		_logger->info() << "Symbol \"" << symbolName << "\" doesn't have an implementation. Replacing with SymbolNotImplemented" << std::endl;
		this->registerLibraryImplementation(
			symbolName,
			std::make_unique<SymbolNotImplemented>(symbolName));
	}

    auto sym = this->_implementations->at(symbolName);

	// * 2, because each symbol has two instructions: UDF and RET
	virtual_address_t jumpAddress = this->_linkingTableAddress.value() + (sym->index * sizeof(uint32_t) * 2);

	const InstructionDefs::Reserved::ReservedCall reservedCall(
			InstructionDefs::Reserved::ReservedCalls::LibraryCall, sym->index);
	const uint32_t encodedResCallInstruction = reservedCall.encode();

	const InstructionDefs::Begsi::UnconditionalBranchRegister ret(
		InstructionDefs::Begsi::UnconditionalBranchRegister::OperationOpc::Return,
		30); // X30 == LR
	const uint32_t encodedRetInstruction = ret.encode();

	memory.write(jumpAddress, encodedResCallInstruction);
	memory.write(jumpAddress + sizeof(uint32_t), encodedRetInstruction);
	return jumpAddress;
}

bool Emulation::Libraries::Mapper::hasLibraryImplementation(Emulation::Libraries::symbol_index_t index) const {
	return this->_indexSymbols->find(index) != this->_indexSymbols->end();
}

EmulatedSymbol&
Emulation::Libraries::Mapper::getLibraryImplementation(Emulation::Libraries::symbol_index_t index) const {
	return *this->_indexSymbols->at(index)->exec.get();
}

void Emulation::Libraries::Mapper::allocateLinkingSegment(CpuVirtualMemory &memory) {
	// Let's just allocate the max size for the sake of simplicity
	// 2^16 == unique numbers expressed by uint16_t
	// * 2 - each lib call has two instructions - reserved call and RET
	// * sizeof(uint32_t) - the two instructions are both uint32_ts
	constexpr size_t segmentSize = (1 << 16) * 2 * sizeof(uint32_t);

	if (this->_linkingTableAddress.has_value()) {
		throw std::runtime_error("Linking table was already allocated");
	}

	_logger->verbose() << "Allocating linking table segment" << std::endl;
	this->_linkingTableAddress = memory.allocateSegment(segmentSize);
	_logger->verbose() << "[Mapper] Linking table segment allocated at " << this->_linkingTableAddress.value()
		<< "; size: " << segmentSize << " bytes" << std::endl;
}
