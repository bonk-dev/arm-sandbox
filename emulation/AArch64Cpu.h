#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include "CpuVirtualMemory.h"
#include "emu_types.h"
#include "CpuStack.h"

constexpr size_t AARCH64_GENERAL_PURPOSE_REGISTERS = 31;
constexpr size_t AARCH64_EMUL_REGISTERS = 8;

class AArch64Cpu {
private:
	std::vector<uint64_t> _emulRegisters;
    std::vector<uint64_t> _generalRegisters;
	uint64_t _programCounter;

	std::vector<uint64_t> _threads;
	std::unique_ptr<CpuVirtualMemory> _memory;
public:
    AArch64Cpu();

	void writeEmulRegister(regindex_t index, uint64_t val);

    void writeGpRegister32(regindex_t index, uint32_t val);
    void writeGpRegister64(regindex_t index, uint64_t val);

	[[nodiscard]] uint64_t getProgramCounter() const;
	void setProgramCounter(uint64_t pc);

	[[nodiscard]] uint64_t readEmulRegister(regindex_t index) const;

    [[nodiscard]] uint32_t readGpRegister32(regindex_t index) const;
    [[nodiscard]] uint64_t readGpRegister64(regindex_t index) const;

	/**
	 * Creates a new thread with a specified id
	 * @param id The new thread id
	 */
	void createThread(uint64_t id);

	[[nodiscard]] CpuVirtualMemory & getMemory() const;
};
