#include "stdafx.h"
#include <iostream>
#include <deque>
#include <windows.h>

#include "utils.h"
#include "CException.h"
#include "PasswordChunk.h"
#include "Hasher.h"
#include "Logger.h"
#include "Fifo.cpp"
#include "PasswordChunk.h"

#include "IHash.h"
#include "CHashNone.h"
#include "CHashCrc32.h"
#include "CHashMd5.h"
#include "CHashSha1.h"
#include "CHashSha224.h"
#include "CHashSha256.h"
#include "CException.h"
#include "start.h"



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
// Au dessus : Code par d�faut du prof

// En dessous : Ajout Alo�s work in progress
// Argument 1
//	CRC32
//	MD5
//	SHA1
//	SHA224
//	SHA256
//
// Argument 2
//	Hash � d�coder
void crackpw(std::string p_hash, std::string target_hash) {
	char password[64] = "";
	std::string alphabet = "abcdefghijklmnopqrstuvwxyz0123456789*"; // Alphabet incomplet. | Bien laisser * � la fin.
	std::string currentHash = "";
	Logger *logger;
	Hasher *hasher;
	Fifo<CPasswordChunk> *pwdFifo = new Fifo<CPasswordChunk>();

	hasher = Hasher::getInstance();
	hasher->initialize(p_hash);
	logger = Logger::getInstance();

	CPasswordChunk pwdChunk;
	pwdChunk.Reset();
	pwdChunk.SetPasswordRange("00000aa", "00000**");
	
	bool passwordFound = false;		// Bool de Mot de passe trouv�
	bool isAborted = false;			// Bool de Arr�t avant mot de passe trouv�
	bool isRunning;					// Bool de Recherche en cours

	const int MAX_PWD_LENGTH = 5;	// Taille max du mot de passe pour incr�menter les chunks. Pour les tests on le laisse petit.
	char currentChunkStart[MAX_PWD_LENGTH] = "";	// Mot de passe pour incr�menter les chunks.

	std::string pwdStartTemp;		// Password de d�but de chunk.
	std::string pwdEndTemp;			// Password de fin de chunk.

	// Boucle Principale
	strcpy_s(password, sizeof(password), "");
	logger->newMessage(0, "Recherche en cours... | " + p_hash + " | " + target_hash);
	do {
		isRunning = true; 

		// Remplissage dela FIFO si taille moins de 3 chunks restants.
		if (pwdFifo->getSize() < 3) {
			// Boucle de remplissage de la FIFO
			while (pwdFifo->getSize() < 10) {
				pwdChunk.Reset();
				HashCrackerUtils::IncreasePassword(currentChunkStart, MAX_PWD_LENGTH, alphabet);

				pwdStartTemp = currentChunkStart;
				pwdStartTemp = pwdStartTemp + "aa";
				pwdEndTemp = currentChunkStart;
				pwdEndTemp = pwdEndTemp + "**";

				pwdChunk.SetPasswordRange(pwdStartTemp, pwdEndTemp);

				pwdFifo->push(pwdChunk);
				logger->newMessage(0, "Nouveau Chunk Injecte : " + pwdStartTemp + " -> " + pwdEndTemp + " | Taille FIFO : " + std::to_string(pwdFifo->getSize()));
			} // Fin boucle remplissage FIFO
		}

		pwdChunk = pwdFifo->pull();
		strcpy_s(password, sizeof(pwdChunk.GetPasswordBegin().c_str()), pwdChunk.GetPasswordBegin().c_str()); // Conversion du string de d�but du chunk en char[]

		// Boucle de traitement d'un chunk
		logger->newMessage(0, "Debut lecture du chunk...");
		do {
			HashCrackerUtils::IncreasePassword(password, sizeof(password), alphabet);
			currentHash = hasher->calculateHash(password);

			//D�commentez cette ligne ci-dessous pour afficher les tentatives une � une :
			//std::cout << password << " -> " << currentHash << "" << std::endl;

			if (currentHash == target_hash) { //CRC32 : "884863D2" = 123 | "2D640152" = 900
				std::string foundMessage = "Trouve ! Le mot de passe est : ";
				logger->newMessage(0, foundMessage.append(password));
				isRunning = false;
				passwordFound = true;
			}

			if (GetAsyncKeyState(VK_ESCAPE) != 0) {
				isRunning = false;
				isAborted = true;
				logger->newMessage(0, "Arret demande par l'utilisateur");
			}

			if (std::string(password) == pwdChunk.GetPasswordEnd()) {
				isRunning = false;
				logger->newMessage(0, "...Fin lecture du chunk | Taille FIFO : " + std::to_string(pwdFifo->getSize()));
			}

		} while (isRunning); // Fin boucle traitement chunk
	} while (!passwordFound && !isAborted); // Fin boucle principale
	return;
}


int main( int argc, const char *argv[] ) {
	std::cout << "Projet Multithreading : Alois - Tristan - Jeremy" << std::endl;
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
