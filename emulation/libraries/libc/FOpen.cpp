#include "FOpen.h"

void Emulation::Libraries::LibC::FOpen::execute(AArch64Cpu &cpu) {
	const virtual_address_t filename_char_ptr = cpu.readRegister64(0);
	const virtual_address_t mode_char_ptr = cpu.readRegister64(1);

	const auto path = cpu.getMemory().readCString(filename_char_ptr);
	const auto mode = cpu.getMemory().readCString(mode_char_ptr);

	_logger->info() << "Opening: " << path << " (mode: " << mode << ")" << std::endl;

	if (!cpu.getFs().doesFileExist(path)) {
		cpu.writeRegister64(0, 0);
	}
	else {
		const auto fileId = cpu.getFs().getFile(path).getFileId();

		Filesystem::VirtualFileStruct fileStruct{fileId};
		const virtual_address_t virtualFileStructAddress = cpu.getMemory().allocateSegment(sizeof(Filesystem::VirtualFileStruct));
		cpu.getMemory().write(virtualFileStructAddress, fileStruct);

		cpu.writeRegister64(0, virtualFileStructAddress);
	}
}
