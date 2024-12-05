#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include "CpuVirtualMemory.h"
#include "emu_types.h"
#include "CpuStack.h"

constexpr size_t AARCH64_GENERAL_PURPOSE_REGISTERS = 32;
constexpr uint64_t AARCH64_CURRENT_THEAD_ID = 0;
constexpr uint64_t AARCH64_MAIN_THREAD_ID = 1;

class AArch64Cpu {
private:
    std::vector<uint64_t> _generalRegisters;
	uint64_t _programCounter;

	CpuVirtualMemory _memory;
	std::map<uint64_t, std::shared_ptr<CpuStack>> _threadStacks;

	std::unique_ptr<CpuStack> _stack;
public:
    explicit AArch64Cpu(size_t initial_memory_size);

    void writeGpRegister32(regindex_t index, uint32_t val);
    void writeGpRegister64(regindex_t index, uint64_t val);

	[[nodiscard]] uint64_t getProgramCounter() const;
	void setProgramCounter(uint64_t pc);

    [[nodiscard]] uint32_t readGpRegister32(regindex_t index) const;
    [[nodiscard]] uint64_t readGpRegister64(regindex_t index) const;

	/**
	 * Creates a new thread
	 * @return The created thread's id
	 */
	uint64_t createThread();

	/**
	 * Creates a new thread with a specified id
	 * @param id The new thread id
	 */
	void createThread(uint64_t id);

	CpuVirtualMemory& getMemory();
	std::shared_ptr<CpuStack> & getStack(uint64_t threadId);
};
