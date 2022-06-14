#pragma once

class CItem
{
protected:
	fstream *m_fsOutFile;
	WIN32_FIND_DATA *m_fdFindData;

public:
	CItem(WIN32_FIND_DATA&, fstream&);
	~CItem();

	void CreationTime();
	void LastAccesstTime();
	void LastWriteTime();
	void LowOrderSize();
	void HighOrderSize();
	void Reserved0();
	void Reserved1();
	void FileName();
	void AlternateFileName();
	// Below parameters were not available from attributes.
	//void filetype();
	//void creatortype();
	//void finderflags();

	bool IsArchive();
	bool IsCompressed();
	bool IsDevice();
	bool IsDirectory();
	bool IsEncrypted();
	bool IsHidden();
	bool IsIntegrityStream();
	bool IsNormal();
	bool IsNotContentIndexed();
	bool IsNoScrubData();
	bool IsOffline();
	bool IsReadonly();
	bool IsRecallOnDataAccess();
	bool IsRecallOnOpen();
	bool IsReparsePoint();
	bool IsSparseFile();
	bool IsSystem();
	bool IsTemporary();
	bool IsVirtual();

	void WriteAllAttributes();

	void WriteAllProperties();
};

/* 
About Class:

Properties: 
	WIN32_FIND_DATA
		- ftCreationTime
		- ftLastAccessTime
		- ftLastWriteTime
		- nFileSizeHigh
		- nFileSizelow
		- dwReserved0
		- dwReserved1
		- cFileName
		- cAlternateFileName
		- dwFileType
		- dwCreatorType
		- wfinderFlags

Attibutes:
	WIN32_FIND_DATA
		- dwFileAttributes (all)
*/

