#pragma once
#include <map>

#include "../CpuVirtualMemory.h"
#include "../emu_types.h"

namespace Emulation::Libraries {
    typedef unsigned long symbol_index_t;
    typedef void (*library_impl_exec_t)();
    typedef struct library_impl_symbol_t {
        library_impl_exec_t exec;
        symbol_index_t index;
    } library_impl_symbol_t;

    class Mapper {
    private:
		std::optional<int> _linkingTableAddress;
        symbol_index_t _nextIndex;
        std::unique_ptr<std::map<const char*, std::shared_ptr<library_impl_symbol_t>>> _implementations;
        std::unique_ptr<std::map<unsigned int, std::shared_ptr<library_impl_symbol_t>>> _indexSymbols;
    public:
        Mapper();
		void allocateLinkingSegment(CpuVirtualMemory& memory);
        void registerLibraryImplementation(const char* symbolName, library_impl_exec_t implementation);
        virtual_address_t mapLibraryImplementation(const char* symbolName, CpuVirtualMemory& memory);
		[[nodiscard]] library_impl_exec_t getLibraryImplementation(symbol_index_t index) const;
    };
}
