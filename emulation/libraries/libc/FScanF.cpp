#include "FScanF.h"

void Emulation::Libraries::LibC::FScanF::execute(AArch64Cpu &cpu) {
	const virtual_address_t fileStructPtr = cpu.readRegister(0, 64);
	const virtual_address_t formatCharPtr = cpu.readRegister(1, 64);
	const virtual_address_t destinationBufferPtr = cpu.readRegister(2, 64);

	_logger->info() << "Reading file struct at " << std::hex << std::showbase << fileStructPtr << std::endl;

	const auto fileStruct = cpu.getMemory().read<Filesystem::VirtualFileStruct>(fileStructPtr);
	if (!fileStruct.isValid()) {
		throw std::runtime_error("File id was invalid");
	}

	auto& file = cpu.getFs().getFile(fileStruct);
	void* fileDataPtr = file.getUnsafeDataPointer();

	std::string formatString = cpu.getMemory().readCString(formatCharPtr);
	const char* fileDataCharPtr = reinterpret_cast<const char*>(fileDataPtr);

	void* virtualDestination = cpu.getMemory().getUnsafePointer(destinationBufferPtr);
	sscanf(fileDataCharPtr, formatString.c_str(), virtualDestination);
}
