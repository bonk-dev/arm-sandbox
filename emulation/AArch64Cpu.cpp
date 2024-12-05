#include <format>
#include "AArch64Cpu.h"

AArch64Cpu::AArch64Cpu(size_t initial_memory_size) : _memory(initial_memory_size),
													 _programCounter(0),
													 _stack(std::make_unique<CpuStack>(8 * 1024 * 1024)) {
    this->_generalRegisters = std::vector<uint64_t>(
        AARCH64_GENERAL_PURPOSE_REGISTERS);
	this->createThread(AARCH64_MAIN_THREAD_ID);
}

void AArch64Cpu::writeGpRegister32(regindex_t index, uint32_t val) {
    this->_generalRegisters[index] = val;
}

void AArch64Cpu::writeGpRegister64(regindex_t index, uint64_t val) {
    this->_generalRegisters[index] = val;
}

uint32_t AArch64Cpu::readGpRegister32(regindex_t index) const {
    return this->_generalRegisters[index];
}

uint64_t AArch64Cpu::readGpRegister64(regindex_t index) const {
    return this->_generalRegisters[index];
}

CpuVirtualMemory &AArch64Cpu::getMemory() {
	return this->_memory;
}

uint64_t AArch64Cpu::getProgramCounter() const {
	return this->_programCounter;
}

void AArch64Cpu::setProgramCounter(uint64_t pc) {
	this->_programCounter = pc;
}

std::shared_ptr<CpuStack> & AArch64Cpu::getStack(uint64_t threadId) {
	if (threadId != AARCH64_CURRENT_THEAD_ID) {
		if (!this->_threadStacks.contains(threadId)) {
			throw std::runtime_error(std::format("Thread {} didn't have a stack", threadId));
		}

		return this->_threadStacks.at(threadId);
	}

	// TODO: Only one thread is supported, so the default thread is always the main thread
	return this->_threadStacks.at(AARCH64_MAIN_THREAD_ID);
}

uint64_t AArch64Cpu::createThread() {
	using pairType = decltype(this->_threadStacks)::value_type;
	auto maxStack = std::max_element(this->_threadStacks.begin(), this->_threadStacks.end(),
					 [](pairType const& a, pairType const& b) {
		return a.first < b.first;
	})->first;

	uint64_t newId = maxStack + 1;
	this->createThread(newId);
	return newId;
}

void AArch64Cpu::createThread(uint64_t id) {
	auto stack = std::make_shared<CpuStack>(10240);
	this->_threadStacks.emplace(id, stack);
}
