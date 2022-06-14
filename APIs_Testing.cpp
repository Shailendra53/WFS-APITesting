// $CVSHeader$
//
// Copyright 2020 Commvault Systems


#include "StdAfx.h"

#include "APIs_Testing.h"

#include "QiUtils/CommandLineArguments.h"
#include "QiUtils/Path.h"

#include <iostream>
#include <fstream>

#include "CItem.h"

using namespace std;

CAPIs_Testing::CAPIs_Testing()
{
}


CAPIs_Testing::~CAPIs_Testing(){
	
}

_Check_return_ CQiError CAPIs_Testing::OpenFile() {

	m_fsOutFile.open(m_strOutFilePath);
	if (!m_fsOutFile.is_open())
		return QIERR_WIN32(ERROR_ERRORS_ENCOUNTERED, "Unable to open the file [%s]", m_strOutFilePath.c_str());

	m_fsBackupReportFile.open(m_strBackupReportFilePath);
	if (!m_fsBackupReportFile.is_open())
		return QIERR_WIN32(ERROR_ERRORS_ENCOUNTERED, "Unable to open the file [%s]", m_strBackupReportFilePath.c_str());

	return QISUCCESS;
}

_Check_return_ CQiError CAPIs_Testing::CloseFile() {

	m_fsOutFile.close();
	m_fsBackupReportFile.close();

	return QISUCCESS;
}

_Check_return_ CQiError CAPIs_Testing::Scan(string strScanPath) {

	string strPathToScan = strScanPath;
	// appending * at the end of path
	FXPath::Append(strPathToScan, "*");

	WIN32_FIND_DATA fdFileData;
	HANDLE FindHandle;

	FindHandle = FindFirstFile(strPathToScan.c_str(), &fdFileData);

	if (FindHandle == INVALID_HANDLE_VALUE) {
		GLOGOUT1(LVL_NORMAL, "FindFirstFile function failed");
		return QISUCCESS;
	}

	do {
		string strNextScanPath = strScanPath;
		FXPath::Append(strNextScanPath, fdFileData.cFileName);

		// Write file path and attributes/properties details to file
		WriteDataToFile(strNextScanPath, fdFileData);
		//// Dump File data to report file
		//ScanFileContentForBackup(strNextScanPath);

		if (fdFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

			if (_tcscmp(fdFileData.cFileName, TEXT(".")) != 0 && 
				_tcscmp(fdFileData.cFileName, TEXT("..")) != 0) {

				// recursive calling for sub directories
				Scan(strNextScanPath);
			}
		}
	} while (FindNextFile(FindHandle, &fdFileData));

	return QISUCCESS;
}

_Check_return_ CQiError CAPIs_Testing::WriteDataToFile(string strFullPath, WIN32_FIND_DATA& fdFileData) {

	m_fsOutFile << strFullPath << endl;
	CItem *cItem = new (std::nothrow) CItem(fdFileData, m_fsOutFile);
	RETURN_QIERR_ON_NEWFAILED(cItem);

	// TODO : Seperate writing part from item object
	cItem->WriteAllAttributes();
	cItem->WriteAllProperties();

	return QISUCCESS;
}

_Check_return_ CQiError CAPIs_Testing::Initialize( _In_ CCommandLineArgumentsPtr ptrArguments )
{
	ptrArguments->GetOption("backupPath", m_strPath);
	ptrArguments->GetOption("collectFilePath", m_strOutFilePath);
	ptrArguments->GetOption("reportFilePath", m_strBackupReportFilePath);

	return QISUCCESS;
}


_Check_return_ CQiError CAPIs_Testing::Run()
{
	GLOGOUT1( LVL_NORMAL, "Starting Listing Directory" );
	GLOGOUT1(LVL_NORMAL, "Opening Files");
	CQiError oError = OpenFile();
	RETURN_ON_QIERR(oError);

	Scan(m_strPath);

	// testing the backup function
	ScanPathContentForBackup(m_strPath);
	ScanCollectFileListForBackup();

	GLOGOUT1(LVL_NORMAL, "Closing Files");
	CloseFile();

	return QISUCCESS;

	// Read and dump a file
}

_Check_return_ CQiError CAPIs_Testing::ScanFileContentForBackup(string strFilePath) {
	
	m_fsBackupReportFile << "############# Start file #############" << endl;
	m_fsBackupReportFile << "\nFileName: " << strFilePath << endl << endl;

	HANDLE hFile = CreateFile(strFilePath.c_str(),		// FileName
							GENERIC_READ,				// DesireAccess
							FILE_SHARE_READ,			// ShareMode
							NULL,						// SecurityAttributes
							OPEN_EXISTING,				// CreationDisposition
							FILE_FLAG_BACKUP_SEMANTICS,	// FlagsAndAttributes
							NULL);						// TemplateFile

	if (hFile == INVALID_HANDLE_VALUE) {
		GLOGOUT1(LVL_NORMAL, "Cannot open File [%s]", strFilePath.c_str());
		return QISUCCESS;
	}

	// The read was improper when used sizof(WIN32_STREAM_ID) = 24
	// So need to hard-code the header size = 20 which is working perfectly.
	DWORD nHeaderSize = 20;
	WIN32_STREAM_ID oHeader;
	const DWORD DEFAULT_BUFFER_SIZE = 4096;
	DWORD dwBytesRead = 0;
	LPVOID lpvContext = NULL;
	BYTE *bBuffer = new BYTE[DEFAULT_BUFFER_SIZE];
	RETURN_QIERR_ON_NEWFAILED(bBuffer);

	do {

		oHeader = { 0 };

		// reading the header.....
		BOOL bRes = BackupRead(hFile, (LPBYTE)&oHeader, nHeaderSize, &dwBytesRead, FALSE, TRUE, &lpvContext);

		if (!bRes || dwBytesRead != nHeaderSize) {
			break;
		}

		LARGE_INTEGER iSizeRead;
		iSizeRead.LowPart = 0;
		iSizeRead.HighPart = 0;

		m_fsBackupReportFile << "====== Start Stream ======\n" << endl;
		m_fsBackupReportFile << "Stream ID  : " << oHeader.dwStreamId << endl;
		m_fsBackupReportFile << "Stream Name: " << BACKUP_STREAMID_MAPPINGS[oHeader.dwStreamId] << endl;
		m_fsBackupReportFile << "Stream Size: " << oHeader.Size.LowPart << endl;
		m_fsBackupReportFile << "Stream Data Follows: " << endl;

		// Checking Header name and reading it.
		if (oHeader.dwStreamNameSize > 0) {

			BYTE *bStreamName = new BYTE[oHeader.dwStreamNameSize];
			RETURN_QIERR_ON_NEWFAILED(bStreamName);

			ZeroMemory(bStreamName, oHeader.dwStreamNameSize);

			bRes = BackupRead(hFile, bStreamName, oHeader.dwStreamNameSize, &dwBytesRead, FALSE, TRUE, &lpvContext);

			if (!bRes || dwBytesRead != oHeader.dwStreamNameSize) {
				break;
			}
		}

		// Reading the Stream data....
		// Using only lowpart to read the file data...
		do {
			ZeroMemory(bBuffer, DEFAULT_BUFFER_SIZE);
			DWORD dwSizeToRead = DEFAULT_BUFFER_SIZE;

			if (oHeader.Size.LowPart - iSizeRead.LowPart < DEFAULT_BUFFER_SIZE) {
				dwSizeToRead = oHeader.Size.LowPart - iSizeRead.LowPart;
			}

			BOOL bRes = BackupRead(hFile, bBuffer, dwSizeToRead, &dwBytesRead, FALSE, TRUE, &lpvContext);

			if (!bRes || dwBytesRead != dwSizeToRead) {
				break;
			}

			m_fsBackupReportFile << bBuffer;

			iSizeRead.LowPart += dwBytesRead;

		} while ( oHeader.Size.LowPart > iSizeRead.LowPart );

		m_fsBackupReportFile << "\n\n======  End  Stream ======" << endl;

	} while ( TRUE );

	BackupRead(hFile, NULL, 0, NULL, TRUE, TRUE, &lpvContext);

	m_fsBackupReportFile << endl << "#############  End  file #############" << endl;

	return QISUCCESS;
}

_Check_return_ CQiError CAPIs_Testing::ScanPathContentForBackup(string strScanPath) {
	string strPathToScan = strScanPath;
	// appending * at the end of path
	FXPath::Append(strPathToScan, "*");

	WIN32_FIND_DATA fdFileData;
	HANDLE FindHandle;

	FindHandle = FindFirstFile(strPathToScan.c_str(), &fdFileData);

	if (FindHandle == INVALID_HANDLE_VALUE) {
		GLOGOUT1(LVL_NORMAL, "FindFirstFile function failed");
		return QISUCCESS;
	}

	do {
		string strNextScanPath = strScanPath;
		FXPath::Append(strNextScanPath, fdFileData.cFileName);

		// Dump File data to report file
		ScanFileContentForBackup(strNextScanPath);

		if (fdFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

			if (_tcscmp(fdFileData.cFileName, TEXT(".")) != 0 &&
				_tcscmp(fdFileData.cFileName, TEXT("..")) != 0) {

				// recursive calling for sub directories
				ScanPathContentForBackup(strNextScanPath);
			}
		}
	} while (FindNextFile(FindHandle, &fdFileData));

	return QISUCCESS;
}

_Check_return_ CQiError CAPIs_Testing::ScanCollectFileListForBackup() {
	m_fsOutFile.seekg(0, ios::beg);
	string strFileline;
	while (!m_fsOutFile.eof()) {
		m_fsOutFile >> strFileline;
		
		// We can call here ScanFileContentForBackup() for scanning content of all fiels in collect file.
	}

	return QISUCCESS;
}

/*
<Stream Header 1><Stream data><Stream Header 2><Stream data><Stream Header 3><Stream data>... END
*/


// BYTE4BYTE3BYTE2BYTE1
// // BYTE4BYTE3BYTE2BYTE1 BYTE4BYTE3BYTE2BYTE1