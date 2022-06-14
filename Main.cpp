// $CVSHeader$
//
// Copyright 2020 Commvault Systems


#include "StdAfx.h"

#include "APIs_Testing.h"

#include "QiUtils/CommandLineArguments.h"
#include "QiUtils/ConsoleOutput.h"
#include "QiUtils/DefaultLogWriter.h"
#if defined(_WIN32)
#include "QiUtils/StringExtensions.h"
#endif // _WIN32

DECLARE_SHARED_PTR(CAPIs_Testing);

#if defined(_WIN32)
#include <delayimp.h>
#endif // _WIN32
#include <iostream>

using namespace ConsoleManip;


static void PrintHelp()
{
	cout << endl;
	cout << "Usage:" << endl << endl;
	cout << pushc << fg_yellow << "APIs_Testing [-help]" << popc << endl;
	cout << endl;
}


_Check_return_ static int _enclosed_main( _In_ int argc, _In_ TCHAR* argv[] )
{
	g_ptrLogWriter = CreateDefaultLogWriter( "APIs_Testing" );

	CCommandLineArgumentsPtr ptrArguments( new CCommandLineArguments( argc, argv ) );

	CQiError oError = ptrArguments->GetOption( "help" );
	if( !oError.IsError() )
	{
		PrintHelp();
		return 0;
	}

	// Initialize and run the implementing class.
	/*while (true) {
		Sleep(1000);
	}
	cout << "Hello world";
	cout << "jlasdhalshdlksaj";*/

	CAPIs_TestingPtr ptrMain( new CAPIs_Testing() );
	if( !ptrMain )
	{
		cout << pushc << fg_red << "Failed to allocate CAPIs_Testing" << popc << endl;
		GERROUT( "Failed to allocate CAPIs_Testing" );
		return 30;
	}

	oError = ptrMain->Initialize( ptrArguments );
	if( oError.IsError() )
	{
		cout << pushc << fg_red << "Initialize failed, error=" << oError.AsString() << popc << endl;
		GERROUT( "Initialize failed, error=%s", oError.AsString().c_str() );
		return 31;
	}

	oError = ptrMain->Run();
	if( oError.IsError() )
	{
		cout << pushc << fg_red << "Run failed, error=" << oError.AsString() << popc << endl;
		GERROUT( "Run failed, error=%s", oError.AsString().c_str() );
		return 32;
	}

	return 0;
}


#if defined(_WIN32)

_Check_return_ static int HandleException( _In_opt_ LPEXCEPTION_POINTERS pExceptionPointers )
{
	if( pExceptionPointers == NULL )
		return 0;

	LPEXCEPTION_RECORD pExceptionRecord = pExceptionPointers->ExceptionRecord;
	if( pExceptionRecord == NULL )
		return 0;

	if( pExceptionRecord->ExceptionCode == VcppException(ERROR_SEVERITY_ERROR, ERROR_MOD_NOT_FOUND) )
	{
		if( pExceptionRecord->NumberParameters >= 1 )
		{
			string strMessage;
			PDelayLoadInfo pInfo = (PDelayLoadInfo)pExceptionRecord->ExceptionInformation[0];
			if( pInfo->dlp.fImportByName )
				strMessage = FXString::Format( "Failed to load delay-load DLL, Module=[%s], Procedure=[%s]", pInfo->szDll, pInfo->dlp.szProcName );
			else
				strMessage = FXString::Format( "Failed to load delay-load DLL, Module=[%s], Ordinal=[%u]", pInfo->szDll, pInfo->dlp.dwOrdinal );

			cout << pushc << fg_red << strMessage << popc << endl;
			GERROUT( "%s", strMessage.c_str() );

			return 1;
		}
	}

	if( pExceptionRecord->ExceptionCode == VcppException(ERROR_SEVERITY_ERROR, ERROR_PROC_NOT_FOUND) )
	{
		if( pExceptionRecord->NumberParameters >= 1 )
		{
			string strMessage;
			PDelayLoadInfo pInfo = (PDelayLoadInfo)pExceptionRecord->ExceptionInformation[0];
			if( pInfo->dlp.fImportByName )
				strMessage = FXString::Format( "Failed to load procedure from delay-load DLL, Module=[%s], Procedure=[%s]", pInfo->szDll, pInfo->dlp.szProcName );
			else
				strMessage = FXString::Format( "Failed to load procedure from delay-load DLL, Module=[%s], Ordinal=[%u]", pInfo->szDll, pInfo->dlp.dwOrdinal );

			cout << pushc << fg_red << strMessage << popc << endl;
			GERROUT( "%s", strMessage.c_str() );

			return 1;
		}
	}

	return 0;
}

#endif // _WIN32


int _tmain( int argc, TCHAR* argv[] )
{
#if defined(_WIN32)
	int iResult = 0;

	__try
	{
		iResult = _enclosed_main( argc, argv );
	}
	__except( HandleException( GetExceptionInformation() ) )
	{
		iResult = 10000;
	}
#else
	int iResult = _enclosed_main( argc, argv );
#endif // _WIN32

	return iResult;
}


