#include "VirtualFileSystem.h"

void Filesystem::VirtualFileSystem::addFile(const std::string& path, std::unique_ptr<Filesystem::File> file) {
	this->_files.emplace(path, std::move(file));
}

Filesystem::VirtualFileSystem::VirtualFileSystem(): _files() {}

Filesystem::File &Filesystem::VirtualFileSystem::getFile(const std::string& path) {
	return *this->_files.at(path);
}

bool Filesystem::VirtualFileSystem::doesFileExist(const std::string &path) const {
	return this->_files.contains(path);
}
