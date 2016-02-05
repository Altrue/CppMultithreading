#include "stdafx.h"
#include <iostream>
#include <pthread.h>
#include "utils.h"
#include "DemoHash.h"
#include <windows.h>

#include "IHash.h"
#include "CHashNone.h"
#include "CHashCrc32.h"
#include "CHashMd5.h"
#include "CHashSha1.h"
#include "CHashSha224.h"
#include "CHashSha256.h"
#include "CException.h"

// Argument 1
//	CRC32
//	MD5
//	SHA1
//	SHA224
//	SHA256
void sandbox(std::string p_hash, std::string target_hash) {
	char password[64] = "";
	std::string testAlphabet = "abcdefghijklmnopqrstuvwxyz0123456789";
	IHash *hasher = nullptr;
	const std::string p_fileName = "";
	std::string currentHash = "";
	
	if (p_hash == "crc32") {
		hasher = new CHashCrc32();
		std::cout << "Hash en CRC32" << std::endl;
	}
	else if (p_hash == "md5") {
		hasher = new CHashMd5();
		std::cout << "Hash en Md5" << std::endl;
	}
	else if (p_hash == "sha1") {
		hasher = new CHashSha1();
		std::cout << "Hash en Sha1" << std::endl;
	}
	else if (p_hash == "sha224") {
		hasher = new CHashSha224();
		std::cout << "Hash en Sha224" << std::endl;
	}
	else if (p_hash == "sha256") {
		hasher = new CHashSha256();
		std::cout << "Hash en Sha256" << std::endl;
	}

	strcpy_s(password, sizeof(password), "");
	bool isRunning = true;
	std::cout << "" << std::endl;
	std::cout << "Recherche en cours..." << std::endl;
	std::cout << "" << std::endl;
	do {
		HashCrackerUtils::IncreasePassword(password, sizeof(password), testAlphabet);
		currentHash = test_hash(hasher, p_fileName, password);
		//std::cout << password << " -> " << currentHash << "" << std::endl;

		if (currentHash == target_hash) {//"884863D2" -> 123 || "2D640152" -> 900
			std::cout << "Trouve ! Le mot de passe est : " << password << std::endl;
			isRunning = false;
		}

		if (GetAsyncKeyState(VK_ESCAPE) != 0) {
			isRunning = false;
			std::cout << "Arret demande par l'utilisateur" << std::endl;
		}

	} while (isRunning);
	return;

	delete hasher;
}


int main( int argc, const char *argv[] ) {
	std::cout << "** Welcome to this sandbox project." << std::endl;
	std::cout << std::endl;
	std::cout << "This is where you can play with libraries and small part of your code" << std::endl << "without impacting the *real* project." << std::endl;
	std::cout << std::endl;

	// Display each command-line argument.
	std::cout << "Command-line arguments:" << std::endl;
	for (int count = 0; count < argc; count++) {
		std::cout << "  argv[" << count << "]   " << argv[count] << std::endl;
	}

	sandbox(argv[1], argv[2]);

	std::cout << std::endl;
	std::cout << "** Goodbye" << std::endl;
	std::cin.get();
	return 0;
}
