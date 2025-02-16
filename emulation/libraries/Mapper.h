#pragma once
#include <map>
#include "../CpuVirtualMemory.h"
#include "../emu_types.h"
#include "EmulatedSymbol.h"
#include <cstring>

namespace Emulation::Libraries {
    typedef unsigned long symbol_index_t;
    typedef std::unique_ptr<EmulatedSymbol> library_impl_exec_t;
    typedef struct library_impl_symbol_t {
        library_impl_exec_t exec;
        symbol_index_t index;
    } library_impl_symbol_t;

    class Mapper {
    private:
        struct str_cmp
        {
            bool operator()(char const *a, char const *b) const
            {
                return std::strcmp(a, b) < 0;
            }
        };

		std::unique_ptr<Logging::LoggerBase> _logger;
		std::optional<virtual_address_t> _linkingTableAddress;
        symbol_index_t _nextIndex;
        std::unique_ptr<std::map<const char*, std::shared_ptr<library_impl_symbol_t>, str_cmp>> _implementations;
        std::unique_ptr<std::map<unsigned int, std::shared_ptr<library_impl_symbol_t>>> _indexSymbols;
    public:
        Mapper();
		void allocateLinkingSegment(CpuVirtualMemory& memory);
        void registerLibraryImplementation(const char* symbolName, library_impl_exec_t implementation);
        virtual_address_t mapLibraryImplementation(const char* symbolName, CpuVirtualMemory& memory);
		[[nodiscard]] bool hasLibraryImplementation(symbol_index_t index) const;
		[[nodiscard]] EmulatedSymbol& getLibraryImplementation(symbol_index_t index) const;
    };
}
