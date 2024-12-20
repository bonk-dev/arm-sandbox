#include "Mapper.h"

#include <stdexcept>

Emulation::Libraries::Mapper::Mapper() :
	_nextIndex(0),
	_indexSymbols(std::make_unique<std::map<unsigned int, std::shared_ptr<library_impl_symbol_t>>>()),
	_implementations(std::make_unique<std::map<const char*, std::shared_ptr<library_impl_symbol_t>>>()) {}

void Emulation::Libraries::Mapper::registerLibraryImplementation(
    const char *symbolName,
    library_impl_exec_t implementation) {
    auto sym = std::make_shared<library_impl_symbol_t>(library_impl_symbol_t{
        std::move(implementation),
        _nextIndex++
    });
    this->_implementations->emplace(symbolName, sym);
    this->_indexSymbols->insert_or_assign(sym->index, sym);
}

virtual_address_t Emulation::Libraries::Mapper::mapLibraryImplementation(const char *symbolName,
    CpuVirtualMemory &memory) {
    // A64:
	// UDF #libraryCall, #symbolIndex

    auto sym = this->_implementations->at(symbolName);

    throw std::runtime_error("Not implemented");
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
	// (2 << 15) - 1 == 65535 (max value of uint16_t, which is the type of ReservedCall's immediate)
	// * sizeof(uint32_t) - each entry in the linking table is a single UDF instruction, and all instructions are of type "uint32_t"
	constexpr size_t segmentSize = ((2 << 15) - 1) * sizeof(uint32_t);

	if (this->_linkingTableAddress.has_value()) {
		throw std::runtime_error("Linking table was already allocated");
	}

	// TODO: maybe allocate in non-user mode area?
	std::cout << "[Mapper] Allocating linking table segment" << std::endl;
	this->_linkingTableAddress = memory.allocateSegment(segmentSize);
	std::cout << "[Mapper] Linking table segment allocated at " << this->_linkingTableAddress.value()
		<< "; size: " << segmentSize << "b" << std::endl;
}
