#include "stdafx.h"
#include <iostream>
#include <pthread.h>

#include "DemoException.h"
#include "DemoHash.h"
#include "DemoSocket.h"
#include "DemoThreads.h"
#include "DemoFiles.h"
#include "DemoDateTime.h"


void demos( int argc, const char *argv[] ) {
	// --- Display parameters received from command line ---
	std::cout << std::endl;
	std::cout << "** Parameters dump:" << std::endl;
	for (int i = 0; i < argc; i++) {
		std::cout << "argv[" << i << "] = \"" << argv[ i ] << "\"" << std::endl;
	}

	// --- Test libraries ---
	std::cout << std::endl;
	//TestThreads();
	TestException();
	//TestHashes();
	//TestSocket();
	//TestFiles();
	//TestDateTime();
	//TestUtil();
}


int main( int argc, const char *argv[] ) {
	std::cout << "** Welcome to this demonstration project." << std::endl;
	std::cout << std::endl;
	std::cout << "It demonstrates how to deal with libraries." << std::endl;
	std::cout << std::endl;

	demos( argc, argv );

	std::cout << std::endl;
	std::cout << "** Goodbye" << std::endl;
	std::cin.get();
	return 0;
}
