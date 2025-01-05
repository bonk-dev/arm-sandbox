#include "EmulatedFile.h"

Filesystem::EmulatedFile::EmulatedFile(const std::string& str):
	Filesystem::File(0), _position(0), _data(str.length()) {
	str.copy(reinterpret_cast<char*>(_data.data()), _data.size());
}

std::string Filesystem::EmulatedFile::readString() {
	std::string s = std::string(reinterpret_cast<char*>(_data.data()));
	_position += s.length();
	return s;
}

void *Filesystem::EmulatedFile::getUnsafeDataPointer() {
	return reinterpret_cast<void*>(_data.data());
}
