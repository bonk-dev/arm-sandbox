#include "Mapper.h"

#include <stdexcept>

Emulation::Libraries::Mapper::Mapper() : _nextIndex(0) {}

void Emulation::Libraries::Mapper::registerLibraryImplementation(
    const char *symbolName,
    library_impl_exec_t implementation) {
    auto sym = std::make_shared<library_impl_symbol_t>(library_impl_symbol_t{
        implementation,
        _nextIndex++
    });
    this->_implementations->emplace(symbolName, sym);
    this->_indexSymbols->insert_or_assign(sym->index, sym);
}

virtual_address_t Emulation::Libraries::Mapper::mapLibraryImplementation(const char *symbolName,
    CpuVirtualMemory &memory) {
    // A64:
    // set EMUL_X0 to symbol index
    // UDF <Emulation::HypervisorCalls:LibraryCall>

    auto sym = this->_implementations->at(symbolName);

    throw std::runtime_error("Not implemented");
}

Emulation::Libraries::library_impl_exec_t
Emulation::Libraries::Mapper::getLibraryImplementation(Emulation::Libraries::symbol_index_t index) const {
	const auto sym = this->_indexSymbols->at(index);
	return sym->exec;
}
