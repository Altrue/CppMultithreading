#include "stdafx.h"
#include <iostream>
#include "DemoFiles.h"

#include "CFileBinary.h"
#include "CFileText.h"
#include "CException.h"


void TestFileText() {
	std::cout << "Starting text-file library tests" << std::endl;
	std::cout << std::endl;
	std::cout << "Using text-file library v" << CFileText::GetVersion() << " as of " << CFileText::GetBuildDate() << "." << std::endl;

	try {
		std::vector<std::string> cnt;

		CFileText fin( "d:\\txt_lf.txt" );
		fin.Open( EFileOpenMode::read );
		cnt = fin.ReadAll();
		std::cout << "----------" << std::endl;
		for (size_t i = 0; i < cnt.size(); i++) {
			std::cout << cnt[ i ] << "~" << std::endl;
		}
		std::cout << "----------" << std::endl;
		fin.Close();

		CFileText fout( "d:\\txt2.txt", EFileOpenMode::write );
		fout.WriteAll( cnt, EFileEOL::UNIX );
		fin.Close();
	}
	catch (CException &e) {			// & is IMPORTANT
		std::cerr << "** --- EXCEPTION THROWN ---" << std::endl;
		std::cerr << "** Type: " << e.GetType() << std::endl;
		std::cerr << "** Message: " << e.GetErrorMessage() << std::endl;
		std::cerr << "** Error code: " << e.GetErrorCode() << std::endl;
		std::cerr << "** Fault location: " << e.GetFaultLocation() << std::endl;
	}
	return;
}


void TestFileBinary() {
	std::cout << "Starting binary-file library tests" << std::endl;
	std::cout << std::endl;
	std::cout << "Using binary-file library v" << CFileBinary::GetVersion() << " as of " << CFileBinary::GetBuildDate() << "." << std::endl;

	try {
		unsigned char buffer[] = "AZERTYUIOPQSDFGHJKLMWXCVBN";

		// Write 15 bytes to file
		CFileBinary fout( "d:\\test.bin" );
		fout.Open( EFileOpenMode::write );
		std::cout << "Written " << fout.WriteData( buffer, 15 ) << " byte(s)." << std::endl;
		fout.Close();

		// Get file size
		CFileBinary fin( "d:\\test.bin" );
		std::cout << "File size is " << fin.GetFileSize() << " byte(s)." << std::endl;
		fin.Open( EFileOpenMode::read );

		// Read 10 bytes from file
		memset( buffer, 0, sizeof( buffer ) );
		std::cout << "Read " << fin.ReadData( 10, buffer, sizeof( buffer ) ) << " byte(s)." << std::endl;
		std::cout << "Data: \"" << buffer << "\"" << std::endl;

		// Try to read 10 bytes from file (only 5 will be OK)
		memset( buffer, 0, sizeof( buffer ) );
		std::cout << "Read " << fin.ReadData( 10, buffer, sizeof( buffer ) ) << " byte(s)." << std::endl;
		std::cout << "Data: \"" << buffer << "\"" << std::endl;
		fin.Close();
	}
	catch (CException &e) {			// & is IMPORTANT
		std::cerr << "** --- EXCEPTION THROWN ---" << std::endl;
		std::cerr << "** Type: " << e.GetType() << std::endl;
		std::cerr << "** Message: " << e.GetErrorMessage() << std::endl;
		std::cerr << "** Error code: " << e.GetErrorCode() << std::endl;
		std::cerr << "** Fault location: " << e.GetFaultLocation() << std::endl;
	}
	return;
}


void TestFiles() {
	TestFileText();
	TestFileBinary();
	return;
}

