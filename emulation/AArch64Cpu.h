#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include "CpuVirtualMemory.h"
#include "emu_types.h"
#include "registers.h"
#include "filesystem/VirtualFileSystem.h"

constexpr size_t AARCH64_GENERAL_PURPOSE_REGISTERS = 31;

class AArch64Cpu {
private:
	uint64_t _nzcvConditionRegister;
    std::vector<uint64_t> _generalRegisters;
	uint64_t _programCounter;

	std::vector<uint64_t> _threads;
	std::unique_ptr<CpuVirtualMemory> _memory;

	virtual_address_t _cleanExitAddress;
	int _exitCode;
	bool _halt;

	Filesystem::VirtualFileSystem _fileSystem;
public:
    AArch64Cpu();

	void writeRegisterSp(regindex_t index, uint64_t val, size_t size);
	void writeRegister(regindex_t index, uint64_t val, size_t size);

	[[nodiscard]] uint64_t readRegisterSp(regindex_t index, size_t size) const;
	[[nodiscard]] uint64_t readRegister(regindex_t index, size_t size) const;

	[[nodiscard]] uint64_t getProgramCounter() const;
	void setProgramCounter(uint64_t pc);

	/**
	 * Reads the nzcv register (condition flags)
	 * @return nzcv register value
	 */
	[[nodiscard]] uint64_t readNzcvRegister() const;

	/**
	 * Writes a new value to the nzcv register, zeroing out bits 27:0 and 63:32 (these are reserved)
	 * @param newValue The new nzcv reg. value
	 */
	void writeNzcvRegsiter(uint64_t newValue);

	/**
	 * Creates a new thread with a specified id
	 * @param id The new thread id
	 */
	void createThread(uint64_t id) const;

	[[nodiscard]] CpuVirtualMemory & getMemory() const;

	[[nodiscard]] virtual_address_t getCleanExitAddress() const;
	void setCleanExitAddress(virtual_address_t address);

    /**
     * Checks whether the execution shall not continue
     * @return True if the execution should be stopped
     */
    [[nodiscard]] bool isHalted() const;

    /**
     * Gets status code returned by the emulated app
     * @return The status code returned by the emulated app
     */
    [[nodiscard]] int getExitCode() const;

    /**
     * Halts further execution of the binary
     * @param statusCode The status code returned by main()
     */
    void haltExecution(int statusCode);

	Filesystem::VirtualFileSystem& getFs();
};
