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


bool ExtractCommandLine( int argc, const char *argv[] )	{
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
		return false;
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

	return true;
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
void crackpw(std::string target_hash, std::string p_algo, std::string p_alphabet, std::string p_chunksize) {
	char password[64] = "";
	std::string alphabet = p_alphabet;
	std::string currentHash = "";
	Logger *logger;
	Hasher *hasher;
	Fifo<CPasswordChunk> *pwdFifo = new Fifo<CPasswordChunk>();

	hasher = Hasher::getInstance();
	hasher->initialize(p_algo);
	logger = Logger::getInstance();
	logger->initialize(3);					// On indique ici le niveau de verbosité du logger.
	logger->newMessage(1, "Lancement du programme.");

	CPasswordChunk pwdChunk;
	
	bool passwordFound = false;				// Bool de Mot de passe trouvé
	bool isAborted = false;					// Bool de Arrêt avant mot de passe trouvé
	bool isRunning;							// Bool de Recherche en cours

	const int MAX_PWD_LENGTH = 10;			// Taille max du mot de passe à trouver. (Rajouter 2)
	const int chunkSize = (atoi(p_chunksize.c_str()) - 2);	// -2 vu qu'on rajoute deux lettres à la fin du chunk quoi qu'il arrive.
	char currentChunkStart[MAX_PWD_LENGTH] = "";	// "Meta" Mot de passe pour incrémenter les chunks.

	// Définition : Un Meta Mot de passe est utilisé pour calculer les chunks. On rajoute "aa" ou "**", pour trouver le début / fin du chunk. Exemple : "hij" = "hijaa" -> "hij**"

	char firstLetter = alphabet.back();		// Dernière lettre de l'alphabet
	std::string firstPwd;					// Premier chunk à trouver, moins 1. On fera "+1" au début de la boucle tout à l'heure.
	for (int i = 1; i < chunkSize && i < MAX_PWD_LENGTH; i++) {	// "" donne un premier chunk de aaa -> a**
		firstPwd += firstLetter;			// "*" donne un premier chunk de "aaaa" -> "aa**", etc...		
	}

	strncpy_s(currentChunkStart, firstPwd.c_str(), _TRUNCATE);

	std::string pwdStartTemp;		// "Meta" Password de début de chunk.
	std::string pwdEndTemp;			// "Meta" Password de fin de chunk.

	// Boucle Principale
	strcpy_s(password, sizeof(password), "");
	logger->newMessage(3, "Recherche en cours... | " + p_algo + " | " + target_hash);
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
				logger->newMessage(4, "Nouveau Chunk Injecte : " + pwdStartTemp + " -> " + pwdEndTemp + " | Taille FIFO : " + std::to_string(pwdFifo->getSize()));

				if (GetAsyncKeyState(VK_ESCAPE) != 0) {
					isAborted = true;
					logger->newMessage(1, "Arret demande par l'utilisateur");
					break;
				}
			} // Fin boucle remplissage FIFO
		}

		if (isAborted) {
			break;
		}

		pwdChunk = pwdFifo->pull();
		strncpy_s(password, pwdChunk.GetPasswordBegin().c_str(), _TRUNCATE); // Conversion du string de début du chunk en char[]

		// Boucle de traitement d'un chunk
		logger->newMessage(4, "Debut lecture du chunk...");
		do {
			HashCrackerUtils::IncreasePassword(password, sizeof(password), alphabet);
			currentHash = hasher->calculateHash(password);

			//Décommentez cette ligne ci-dessous pour afficher les tentatives une à une :
			//std::cout << password << " -> " << currentHash << "" << std::endl;

			if (currentHash == target_hash) { //CRC32 : "884863D2" = 123 | "2D640152" = 900
				std::string foundMessage = "Trouve ! Le mot de passe est : ";
				logger->newMessage(3, foundMessage.append(password));
				isRunning = false;
				passwordFound = true;
			}

			if (std::string(password) == pwdChunk.GetPasswordEnd()) {
				isRunning = false;
				logger->newMessage(4, "...Fin lecture du chunk | Taille FIFO : " + std::to_string(pwdFifo->getSize()));
			}

			if (GetAsyncKeyState(VK_ESCAPE) != 0) {
				isRunning = false;
				isAborted = true;
				logger->newMessage(0, "Arret demande par l'utilisateur");
			}
		} while (isRunning); // Fin boucle traitement chunk
	} while (!passwordFound && !isAborted); // Fin boucle principale
}


int main( int argc, const char *argv[] ) {
	std::cout << "Projet Multithreading : Alois - Tristan - Jeremy" << std::endl;
	std::cout << std::endl;

	//GeneratePasswords();
	//EnqueueDequeue();

	// On vérifie si les arguments sont bon ou pas.
	if (ExtractCommandLine(argc, argv)) {
		crackpw(argv[2], argv[4], argv[6], argv[8]);
	}
	else {
		std::cout << std::endl;
		std::cout << "Mauvais arguments de ligne de commande." << std::endl;

		// Display each command-line argument.
		std::cout << "Voici les arguments trouvés :" << std::endl;
		for (int count = 0; count < argc; count++) {
			std::cout << "  argv[" << count << "]   " << argv[count] << std::endl;
		}
		// Arguments à mettre : -hash 884863D2 -algo crc32 -alphabet abcdefghijklmnopqrstuvwxyz0123456789* -chunksize 4
	}

	std::cout << std::endl;
	std::cout << "Fermeture du programme." << std::endl;
	std::cin.get();
	return EXIT_SUCCESS;
}
