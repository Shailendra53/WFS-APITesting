#include "stdafx.h"
#include "FileAttributeConstants.h"

bool FileAttributeConstants::isArchive(DWORD FileAttributes) {
	// A file or directory that is an archive file or directory. Applications typically use this attribute to mark files for backup or removal . 
	return FileAttributes & FILE_ATTRIBUTE_ARCHIVE;
}

bool FileAttributeConstants::isCompressed(DWORD FileAttributes) {
	// compressed file 
	return FileAttributes & FILE_ATTRIBUTE_COMPRESSED;
}

bool FileAttributeConstants::isDevice(DWORD FileAttributes) {
	// value reserved for system use 
	return FileAttributes & FILE_ATTRIBUTE_DEVICE;
}

bool FileAttributeConstants::isDirectory(DWORD FileAttributes) {
	// idenfies a directory 
	return FileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}

bool FileAttributeConstants::isEncrypted(DWORD FileAttributes) {
	// encrypted file or directory 
	return FileAttributes & FILE_ATTRIBUTE_ENCRYPTED;
}

bool FileAttributeConstants::isHidden(DWORD FileAttributes) {
	// hidden file or directory 
	return FileAttributes & FILE_ATTRIBUTE_HIDDEN;
}

bool FileAttributeConstants::isIntegrityStream(DWORD FileAttributes) {
	/* The directory or user data stream is configured with integrity (only supported on ReFS volumes). */
	return FileAttributes & FILE_ATTRIBUTE_INTEGRITY_STREAM;
}

bool FileAttributeConstants::isNormal(DWORD FileAttributes) {
	/* The directory or user data stream is configured with integrity (only supported on ReFS volumes). */
	return FileAttributes & FILE_ATTRIBUTE_NORMAL;
}

bool FileAttributeConstants::isNotContentIndexed(DWORD FileAttributes) {
	/* The directory or user data stream is configured with integrity (only supported on ReFS volumes). */
	return FileAttributes & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
}

bool FileAttributeConstants::isNoScrubData(DWORD FileAttributes) {
	/* The directory or user data stream is configured with integrity (only supported on ReFS volumes). */
	return FileAttributes & FILE_ATTRIBUTE_NO_SCRUB_DATA;
}

bool FileAttributeConstants::isOffline(DWORD FileAttributes) {
	/* The directory or user data stream is configured with integrity (only supported on ReFS volumes). */
	return FileAttributes & FILE_ATTRIBUTE_OFFLINE;
}

bool FileAttributeConstants::isReadonly(DWORD FileAttributes) {
	/* The directory or user data stream is configured with integrity (only supported on ReFS volumes). */
	return FileAttributes & FILE_ATTRIBUTE_READONLY;
}
/*
bool FileAttributeConstants::isRecallOnDataAccess(DWORD FileAttributes) {
	// TODO: description
	return FileAttributes & FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS;
}

bool FileAttributeConstants::isRecallOnOpen(DWORD FileAttributes) {
	// TODO: description
	return FileAttributes & FILE_ATTRIBUTE_RECALL_ON_OPEN;
}
*/
bool FileAttributeConstants::isReparsePoint(DWORD FileAttributes) {
	// TODO: description
	return FileAttributes & FILE_ATTRIBUTE_REPARSE_POINT;
}

bool FileAttributeConstants::isSparseFile(DWORD FileAttributes) {
	// TODO: description
	return FileAttributes & FILE_ATTRIBUTE_SPARSE_FILE;
}

bool FileAttributeConstants::isSystem(DWORD FileAttributes) {
	// TODO: description
	return FileAttributes & FILE_ATTRIBUTE_SYSTEM;
}

bool FileAttributeConstants::isTemporary(DWORD FileAttributes) {
	// TODO: description
	return FileAttributes & FILE_ATTRIBUTE_TEMPORARY;
}

bool FileAttributeConstants::isVirtual(DWORD FileAttributes) {
	// TODO: description
	return FileAttributes & FILE_ATTRIBUTE_VIRTUAL;
}

void FileAttributeConstants::checkAllAttributes(DWORD FileAttributes, string filename) {
	ofstream file;
	file.open(filename, ios::app);

	file << "  Attributes: " << endl;
	file << "    Archive [" << this->isArchive(FileAttributes) << "]" << endl;
	file << "    Compressed [" << this->isCompressed(FileAttributes) << "]" << endl;
	file << "    Device [" << this->isDevice(FileAttributes) << "]" << endl;
	file << "    Directory [" << this->isDirectory(FileAttributes) << "]" << endl;
	file << "    Encrypted [" << this->isEncrypted(FileAttributes) << "]" << endl;
	file << "    Hidden [" << this->isHidden(FileAttributes) << "]" << endl;
	file << "    Integrity System [" << this->isIntegrityStream(FileAttributes) << "]" << endl;
	file << "    Normal [" << this->isNormal(FileAttributes) << "]" << endl;
	file << "    Not Content Indexed [" << this->isNotContentIndexed(FileAttributes) << "]" << endl;
	file << "    No Scrub Data [" << this->isNoScrubData(FileAttributes) << "]" << endl;
	file << "    Offline [" << this->isOffline(FileAttributes) << "]" << endl;
	file << "    Readonly [" << this->isReadonly(FileAttributes) << "]" << endl;
	//file << "  Recall on Data Access [" << this->isRecallOnDataAccess(FileAttributes) << "]" << endl;
	//file << "  Recall on open [" << this->isRecallOnOpen(FileAttributes) << "]" << endl;
	file << "    Reparse Point [" << this->isReparsePoint(FileAttributes) << "]" << endl;
	file << "    Sparse File [" << this->isSparseFile(FileAttributes) << "]" << endl;
	file << "    System [" << this->isSystem(FileAttributes) << "]" << endl;
	file << "    Temporary [" << this->isTemporary(FileAttributes) << "]" << endl;
	file << "    Virtual [" << this->isVirtual(FileAttributes) << "]" << endl;

	file.close();
}