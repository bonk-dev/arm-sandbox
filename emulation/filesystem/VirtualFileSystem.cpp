#include "VirtualFileSystem.h"

Filesystem::VirtualFileSystem::VirtualFileSystem(): _filesByPath(), _availableFileId(1) {}

void Filesystem::VirtualFileSystem::addFile(const std::string& path, std::shared_ptr<Filesystem::File> file) {
	file->setFileId(_takeNextFileId());
	this->_filesByPath.emplace(path, file);
	this->_filesById.emplace(file->getFileId(), file);
}

Filesystem::File &Filesystem::VirtualFileSystem::getFile(const std::string& path) {
	return *this->_filesByPath.at(path);
}

bool Filesystem::VirtualFileSystem::doesFileExist(const std::string &path) const {
	return this->_filesByPath.contains(path);
}

uint64_t Filesystem::VirtualFileSystem::_takeNextFileId() {
	return _availableFileId++;
}

Filesystem::File &Filesystem::VirtualFileSystem::getFile(const Filesystem::VirtualFileStruct& fileStruct) {
	return *this->_filesById[fileStruct.file_id];
}
