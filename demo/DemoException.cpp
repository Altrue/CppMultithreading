#include "stdafx.h"
#include <iostream>
#include "DemoException.h"

#include "CException.h"


void TestException() {
	std::cout << std::endl;
	std::cout << "--------------------------------------------------" << std::endl;

	std::cout << "Using exception library v" << CException::GetVersion() << " as of " << CException::GetBuildDate() << "." << std::endl;

	// Use RTII to get default exception type
	std::cout << CException::CLASS_TYPE << std::endl;

	try {
		std::cout << std::endl;
		std::cout << "Trying impossible..." << std::endl;
		throw _CException( "Wow! something went wrong.", 1212 );
		// Code below will never be run
		std::cout << "You will never see this text displayed." << std::endl;
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
