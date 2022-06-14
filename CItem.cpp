#include "stdafx.h"
#include "CItem.h"

CItem::CItem(WIN32_FIND_DATA& fdFindData, fstream& fsFileName) {
	m_fdFindData = &fdFindData;
	m_fsOutFile = &fsFileName;
}

CItem::~CItem() {

}

void CItem::CreationTime() {
	SYSTEMTIME stTime;

	if (FileTimeToSystemTime(&((*m_fdFindData).ftCreationTime), &stTime)) {
		*m_fsOutFile << "    Creation Time - [" << stTime.wDay << "/" << stTime.wMonth << "/"
			<< stTime.wYear << "T" << stTime.wHour << ":" << stTime.wMinute << ":"
			<< stTime.wSecond << "]\n";
	}
	else {
		GLOGOUT1(LVL_NORMAL, "FileTimeToSystemTime Failed");
	}
}

void CItem::LastAccesstTime() {
	SYSTEMTIME stTime;

	if (FileTimeToSystemTime(&((*m_fdFindData).ftLastAccessTime), &stTime)) {
		*m_fsOutFile << "    Last Access Time - [" << stTime.wDay << "/" << stTime.wMonth << "/"
			<< stTime.wYear << "T" << stTime.wHour << ":" << stTime.wMinute << ":"
			<< stTime.wSecond << "]\n";
	}
	else {
		GLOGOUT1(LVL_NORMAL, "FileTimeToSystemTime Failed");
	}
}

void CItem::LastWriteTime() {
	SYSTEMTIME stTime;

	if (FileTimeToSystemTime(&((*m_fdFindData).ftLastWriteTime), &stTime)) {
		*m_fsOutFile << "    Last Write Time - [" << stTime.wDay << "/" << stTime.wMonth << "/"
			<< stTime.wYear << "T" << stTime.wHour << ":" << stTime.wMinute << ":"
			<< stTime.wSecond << "]\n";
	}
	else {
		GLOGOUT1(LVL_NORMAL, "FileTimeToSystemTime Failed");
	}
}

void CItem::LowOrderSize() {

	*m_fsOutFile << "    Low Order Size - [" << (*m_fdFindData).nFileSizeLow << "]" << endl;
}

void CItem::HighOrderSize() {

	*m_fsOutFile << "    High Order Size - [" << (*m_fdFindData).nFileSizeHigh << "]" << endl;
}

void CItem::Reserved0() {

	if ((*m_fdFindData).dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
		*m_fsOutFile << "    Reserved0 - [" << (*m_fdFindData).dwReserved0 << "]" << endl;
	}
}

void CItem::Reserved1() {

	*m_fsOutFile << "    Reserved1 - [" << (*m_fdFindData).dwReserved1 << "]" << endl;
}

void CItem::FileName() {

	*m_fsOutFile << "    File Name - [" << (*m_fdFindData).cFileName << "]" << endl;
}

void CItem::AlternateFileName() {

	*m_fsOutFile << "    Alternate File Name - [" << (*m_fdFindData).cAlternateFileName << "]" << endl;
}

/* These attributes are not available in WIN32_FILE_DATA

void CItem::FileType() {

	*m_fsOutFile << "    File Type - [" << (*m_fdFindData).filetyp << "]" << endl;
}

void CItem::CreatorType() {

	*m_fsOutFile << "    Creator Type - [" << dwCreatorType << "]" << endl;
}

void CItem::FinderFlags() {

	*m_fsOutFile << "    Flags - [" << dwFinderFlags << "]" << endl;
}
*/

bool CItem::IsArchive() {
	// A file or directory that is an archive file or directory. 
	// Applications typically use this attribute to mark files for backup or removal . 
	return (*m_fdFindData).dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE;
}

bool CItem::IsCompressed() {
	// compressed file 
	return (*m_fdFindData).dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED;
}

bool CItem::IsDevice() {
	// value reserved for system use 
	return (*m_fdFindData).dwFileAttributes & FILE_ATTRIBUTE_DEVICE;
}

bool CItem::IsDirectory() {
	// idenfies a directory 
	return (*m_fdFindData).dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}

bool CItem::IsEncrypted() {
	// encrypted file or directory 
	return (*m_fdFindData).dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED;
}

bool CItem::IsHidden() {
	// hidden file or directory 
	return (*m_fdFindData).dwFileAttributes & FILE_ATTRIBUTE_HIDDEN;
}

bool CItem::IsIntegrityStream() {
	// TODO: description
	return (*m_fdFindData).dwFileAttributes & FILE_ATTRIBUTE_INTEGRITY_STREAM;
}

bool CItem::IsNormal() {
	// TODO: description
	return (*m_fdFindData).dwFileAttributes & FILE_ATTRIBUTE_NORMAL;
}

bool CItem::IsNotContentIndexed() {
	// TODO: description
	return (*m_fdFindData).dwFileAttributes & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
}

bool CItem::IsNoScrubData() {
	// TODO: description
	return (*m_fdFindData).dwFileAttributes & FILE_ATTRIBUTE_NO_SCRUB_DATA;
}

bool CItem::IsOffline() {
	// TODO: description
	return (*m_fdFindData).dwFileAttributes & FILE_ATTRIBUTE_OFFLINE;
}

bool CItem::IsReadonly() {
	// TODO: description
	return (*m_fdFindData).dwFileAttributes & FILE_ATTRIBUTE_READONLY;
}

bool CItem::IsRecallOnDataAccess() {
	// TODO: description
	return (*m_fdFindData).dwFileAttributes & FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS;
}

bool CItem::IsRecallOnOpen() {
	// TODO: description
	return (*m_fdFindData).dwFileAttributes & FILE_ATTRIBUTE_RECALL_ON_OPEN;
}

bool CItem::IsReparsePoint() {
	// TODO: description
	return (*m_fdFindData).dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT;
}

bool CItem::IsSparseFile() {
	// TODO: description
	return (*m_fdFindData).dwFileAttributes & FILE_ATTRIBUTE_SPARSE_FILE;
}

bool CItem::IsSystem() {
	// TODO: description
	return (*m_fdFindData).dwFileAttributes & FILE_ATTRIBUTE_SYSTEM;
}

bool CItem::IsTemporary() {
	// TODO: description
	return (*m_fdFindData).dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY;
}

bool CItem::IsVirtual() {
	// TODO: description
	return (*m_fdFindData).dwFileAttributes & FILE_ATTRIBUTE_VIRTUAL;
}

void CItem::WriteAllAttributes() {

	*m_fsOutFile << "  Attributes: " << endl;
	*m_fsOutFile << "    Archive [" << this->IsArchive() << "]" << endl;
	*m_fsOutFile << "    Compressed [" << this->IsCompressed() << "]" << endl;
	*m_fsOutFile << "    Device [" << this->IsDevice() << "]" << endl;
	*m_fsOutFile << "    Directory [" << this->IsDirectory() << "]" << endl;
	*m_fsOutFile << "    Encrypted [" << this->IsEncrypted() << "]" << endl;
	*m_fsOutFile << "    Hidden [" << this->IsHidden() << "]" << endl;
	*m_fsOutFile << "    Integrity System [" << this->IsIntegrityStream() << "]" << endl;
	*m_fsOutFile << "    Normal [" << this->IsNormal() << "]" << endl;
	*m_fsOutFile << "    Not Content Indexed [" << this->IsNotContentIndexed() << "]" << endl;
	*m_fsOutFile << "    No Scrub Data [" << this->IsNoScrubData() << "]" << endl;
	*m_fsOutFile << "    Offline [" << this->IsOffline() << "]" << endl;
	*m_fsOutFile << "    Readonly [" << this->IsReadonly() << "]" << endl;
	*m_fsOutFile << "    Recall on Data Access [" << this->IsRecallOnDataAccess() << "]" << endl;
	*m_fsOutFile << "    Recall on open [" << this->IsRecallOnOpen() << "]" << endl;
	*m_fsOutFile << "    Reparse Point [" << this->IsReparsePoint() << "]" << endl;
	*m_fsOutFile << "    Sparse File [" << this->IsSparseFile() << "]" << endl;
	*m_fsOutFile << "    System [" << this->IsSystem() << "]" << endl;
	*m_fsOutFile << "    Temporary [" << this->IsTemporary() << "]" << endl;
	*m_fsOutFile << "    Virtual [" << this->IsVirtual() << "]" << endl;
}

void CItem::WriteAllProperties() {

	*m_fsOutFile << "  Properties:" << endl;
	this->FileName();
	this->AlternateFileName();
	this->CreationTime();
	this->LastAccesstTime();
	this->LastWriteTime();
	this->LowOrderSize();
	this->HighOrderSize();
	this->Reserved0();
	//this->FileType(FileData.defilety);
	//this->CreatorType(FileData.creato);
	//this->FinderFlags(FileData.flag);
}