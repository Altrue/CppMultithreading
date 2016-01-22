#include "stdafx.h"
#include <iostream>
#include <pthread.h>


void sandbox() {
	// Add here all your tests
	return;
}


int main( int argc, const char *argv[] ) {
	std::cout << "** Welcome to this sandbox project." << std::endl;
	std::cout << std::endl;
	std::cout << "This is where you can play with libraries and small part of your code" << std::endl << "without impacting the *real* project." << std::endl;
	std::cout << std::endl;

	sandbox();

	std::cout << std::endl;
	std::cout << "** Goodbye" << std::endl;
	std::cin.get();
	return 0;
}
