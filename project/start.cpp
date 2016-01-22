#include "stdafx.h"
#include <iostream>
#include <deque>

#include "utils.h"
#include "CException.h"
#include "PasswordChunk.h"


void ExtractCommandLine( int argc, const char *argv[] )	{
	// Command line settings
	// Tip: good to be stored in a singleton dedicated to configuration	--> see singleton.h
	std::string hash, algo, alphabet, masterIpAddress;
	unsigned int chunkSize = 0;
	bool runningAsSlave = false;

	// Extract command line
	try {
		HashCrackerUtils::ParseCommandLine(argc, argv, hash, algo, alphabet, chunkSize, masterIpAddress, runningAsSlave);
	}
	catch(CException &ex) {
		std::cerr << "** Command line extraction failed at \"" << ex.GetFaultLocation() << "\" with error code " << ex.GetErrorCode() << " and message \"" << ex.GetErrorMessage() << "\"" << std::endl;
		return;
	}

	// Show information
	std::cout << "--- INFORMATION GOT FROM COMMAND LINBE ---" << std::endl;
	std::cout << "Mode: " << (runningAsSlave ? "slave" : "master") << std::endl;
	if( runningAsSlave ) {
		std::cout << "-ip " << masterIpAddress << std::endl;
	}
	else {
		std::cout << "-hash " << hash << std::endl;
		std::cout << "-algo " << algo << std::endl;
		std::cout << "-alphabet " << alphabet << std::endl;
		std::cout << "-chunksize " << chunkSize << std::endl;
	}
}


void GeneratePasswords() {
	char password[64] = "";
	std::string testAlphabet = "0123456789";

	strcpy_s(password, sizeof(password), "");
	for( int i = 0; i < 2500; i++ ) {
		HashCrackerUtils::IncreasePassword(password, sizeof(password), testAlphabet);
		std::cout << "New password: \"" << password << "\"" << std::endl;
	}
}


void EnqueueDequeue() {
	std::deque<CPasswordChunk> fifo;
	CPasswordChunk chunk;

	fifo.clear();
	
	std::cout << "Queuing 00000aa --> 00000**" << std::endl;
	chunk.Reset();
	chunk.SetPasswordRange( "00000aa", "00000**" );
	fifo.push_back( chunk );

	std::cout << "Queuing 00001aa --> 00001**" << std::endl;
	chunk.Reset();
	chunk.SetPasswordRange( "00001aa", "00001**" );
	fifo.push_back( chunk );

	std::cout << "Element count in FIFO: " << fifo.size() << std::endl;
	while (fifo.size() > 0)
	{
		chunk.Reset();
		chunk = fifo.front();
		fifo.pop_front();
		std::cout << "Poped element: password range [" << chunk.GetPasswordBegin() << ", " << chunk.GetPasswordEnd() << "]" << std::endl;
	}
	std::cout << "Element count in FIFO: " << fifo.size() << std::endl;
}


int main( int argc, const char *argv[] ) {
	std::cout << "** Welcome to this project skeleton." << std::endl;
	std::cout << "This is where you need to code the hash cracker." << std::endl;
	std::cout << std::endl;

	//ExtractCommandLine( argc, argv );
	GeneratePasswords();
	//EnqueueDequeue();

	std::cout << std::endl;
	std::cout << "** Goodbye" << std::endl;
	std::cin.get();
	return EXIT_SUCCESS;
}
