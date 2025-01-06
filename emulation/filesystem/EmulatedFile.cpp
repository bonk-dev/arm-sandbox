#include "EmulatedFile.h"

Filesystem::EmulatedFile::EmulatedFile(const std::string& str):
	Filesystem::File(0),
	_position(0),
	_data(str.length() + 1) {

	str.copy(reinterpret_cast<char*>(_data.data()), _data.size());
	// Ensure the string is null-terminated
	_data[str.length()] = static_cast<std::byte>('\x00');
}

std::string Filesystem::EmulatedFile::readString() {
	std::string s = std::string(reinterpret_cast<char*>(_data.data()));
	_position += s.length();
	return s;
}

void *Filesystem::EmulatedFile::getUnsafeDataPointer() {
	return reinterpret_cast<void*>(_data.data());
}
