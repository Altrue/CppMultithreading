#include "stdafx.h"
#include <iostream>
#include <typeinfo>

#include "DemoHash.h"

#include "CException.h"

#include "IHash.h"
#include "CHashNone.h"
#include "CHashCrc32.h"
#include "CHashMd5.h"
#include "CHashSha1.h"
#include "CHashSha224.h"
#include "CHashSha256.h"
#include "CException.h"


void test_hash( IHash *p_hasher, const std::string p_fileName, const std::string p_text ) {
	std::cout << "Testing hash library " << typeid(p_hasher).name() << "." << std::endl;

	// Info
	std::cout << "- Hash size: " << p_hasher->GetHashSize() << std::endl;
	std::cout << std::endl;

	// From file
	std::cout << "- From file \"" << p_fileName << "\"..." << std::endl;
	try {
		p_hasher->HashFile( p_fileName );
		std::cout << "- Hash is " << p_hasher->GetHash() << std::endl;
	}
	catch (CException &e) {
		std::cerr << "** --- EXCEPTION THROWN ---" << std::endl;
		std::cerr << "** Type: " << e.GetType() << std::endl;
		std::cerr << "** Message: " << e.GetErrorMessage() << std::endl;
		std::cerr << "** Error code: " << e.GetErrorCode() << std::endl;
		std::cerr << "** Fault location: " << e.GetFaultLocation() << std::endl;
	}
	std::cout << std::endl;

	// From buffer
	std::cout << "- From buffer..." << std::endl;
	try {
		unsigned char rawHash[ 128 ];

		p_hasher->HashBuffer( reinterpret_cast<const unsigned char *>(p_text.c_str()), static_cast<int>(p_text.length()) );
		std::cout << "- Hash is " << p_hasher->GetHash() << std::endl;
		p_hasher->GetRawHash( &rawHash[ 0 ], sizeof(rawHash) );
	}
	catch (CException &e) {
		std::cerr << "** --- EXCEPTION THROWN ---" << std::endl;
		std::cerr << "** Type: " << e.GetType() << std::endl;
		std::cerr << "** Message: " << e.GetErrorMessage() << std::endl;
		std::cerr << "** Error code: " << e.GetErrorCode() << std::endl;
		std::cerr << "** Fault location: " << e.GetFaultLocation() << std::endl;
	}
	std::cout << std::endl << std::endl << std::endl;
}


void TestHashes() {
	IHash *hasher = nullptr;
	const std::string p_fileName = "C:\\Windows\\WindowsUpdate.log";
	const std::string p_text = "I'm a string ready to be hashed";


	hasher = new CHashCrc32();
	test_hash( hasher, p_fileName, p_text );
	delete hasher;

	hasher = new CHashMd5();
	test_hash( hasher, p_fileName, p_text );
	delete hasher;

	hasher = new CHashSha1();
	test_hash( hasher, p_fileName, p_text );
	delete hasher;

	hasher = new CHashSha224();
	test_hash( hasher, p_fileName, p_text );
	delete hasher;

	hasher = new CHashSha256();
	test_hash( hasher, p_fileName, p_text );
	delete hasher;
}
