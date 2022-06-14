// $CVSHeader$
//
// Copyright 2020 Commvault Systems


#if !defined(_APIS_TESTING_H_960E155C_D75D_4C01_A14E_E4D83F97D186)
#define _APIS_TESTING_H_960E155C_D75D_4C01_A14E_E4D83F97D186

#if defined(_MSC_VER)
#pragma once
#endif // _MSC_VER

DECLARE_SHARED_PTR(CCommandLineArguments);

static string BACKUP_STREAMID_MAPPINGS[] = {
	"BACKUP_INVALID",
	"BACKUP_DATA",
	"BACKUP_EA_DATA",
	"BACKUP_SECURITY_DATA",
	"BACKUP_ALTERNATE_DATA",
	"BACKUP_LINK",
	"BACKUP_PROPERTY_DATA",
	"BACKUP_OBJECT_ID",
	"BACKUP_REPARSE_DATA",
	"BACKUP_SPARSE_BLOCK",
	"BACKUP_TXFS_DATA",
	"BACKUP_GHOSTED_FILE_EXTENTS"
};

class CAPIs_Testing
{
public:

	CAPIs_Testing();
	~CAPIs_Testing();

	_Check_return_ CQiError Initialize( _In_ CCommandLineArgumentsPtr ptrArguments );
	_Check_return_ CQiError Run();

protected:
	string m_strPath;
	string m_strOutFilePath;
	string m_strBackupReportFilePath;
	fstream m_fsOutFile;
	fstream m_fsBackupReportFile;

	_Check_return_ CQiError OpenFile();
	_Check_return_ CQiError Scan(string);
	_Check_return_ CQiError CloseFile();
	_Check_return_ CQiError WriteDataToFile(string, WIN32_FIND_DATA&);

	// backup related functions
	_Check_return_ CQiError ScanFileContentForBackup(string);
	_Check_return_ CQiError ScanPathContentForBackup(string);
	_Check_return_ CQiError ScanCollectFileListForBackup();
};


#endif // _APIS_TESTING_H_960E155C_D75D_4C01_A14E_E4D83F97D186
