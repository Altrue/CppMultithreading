#include "stdafx.h"
#include <iostream>
#include <deque>
#include <windows.h>

#include "utils.h"
#include "CException.h"
#include "PasswordChunk.h"
#include "Hasher.h"

#include "IHash.h"
#include "CHashNone.h"
#include "CHashCrc32.h"
#include "CHashMd5.h"
#include "CHashSha1.h"
#include "CHashSha224.h"
#include "CHashSha256.h"
#include "CException.h"



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
// Au dessus : Code par défaut du prof

// En dessous : Ajout Aloïs work in progress
// Argument 1
//	CRC32
//	MD5
//	SHA1
//	SHA224
//	SHA256
//
// Argument 2
//	Hash à décoder
void crackpw(std::string p_hash, std::string target_hash) {
	char password[64] = "";
	std::string testAlphabet = "abcdefghijklmnopqrstuvwxyz0123456789";
	IHash *hasher = nullptr;
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
		currentHash = calculateHash(hasher, password);
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
	std::cout << "** Welcome to this project skeleton." << std::endl;
	std::cout << "This is where you need to code the hash cracker." << std::endl;
	std::cout << std::endl;

	//ExtractCommandLine( argc, argv );
	//GeneratePasswords();
	//EnqueueDequeue();

	crackpw(argv[1], argv[2]);

	std::cout << std::endl;
	std::cout << "** Goodbye" << std::endl;
	std::cin.get();
	return EXIT_SUCCESS;
}
