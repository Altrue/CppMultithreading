#include "stdafx.h"
#include <iostream>
#include <deque>
#include <windows.h>

#include "utils.h"
#include "CException.h"
#include "Hasher.h"
#include "Logger.h"
#include "Fifo.cpp"
#include "PasswordChunk.h"
#include "LocalOrdonnancer.h"
#include "Context.h"

#include "IHash.h"
#include "CHashNone.h"
#include "CHashCrc32.h"
#include "CHashMd5.h"
#include "CHashSha1.h"
#include "CHashSha224.h"
#include "CHashSha256.h"
#include "CException.h"
#include "start.h"
#include "CUtil.h"


int ExtractCommandLine( int argc, const char *argv[] )	{
	// Command line settings
	// Tip: good to be stored in a singleton dedicated to configuration	--> see singleton.h
	std::string hash, algo, alphabet, masterIpAddress;
	unsigned int chunkSize = 0;
	bool runningAsSlave = false;
	bool forceNew = false;
	int returnValue = 0;

	// Extract command line
	try {
		HashCrackerUtils::ParseCommandLine(argc, argv, hash, algo, alphabet, chunkSize, masterIpAddress, runningAsSlave, forceNew);
	}
	catch(CException &ex) {
		std::cerr << "** Command line extraction failed at \"" << ex.GetFaultLocation() << "\" with error code " << ex.GetErrorCode() << " and message \"" << ex.GetErrorMessage() << "\"" << std::endl;
		return returnValue;
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
		if (chunkSize < 3) {
			std::cout << "ATTENTION : Chunk size minimum de 3" << std::endl;
			chunkSize = 3;
		}
		std::cout << "-chunksize " << chunkSize << std::endl;
		std::cout << "Ignorer précédente tentative interrompue : " << (forceNew == true ? "oui" : "non") << std::endl;
	}

	forceNew == true ? returnValue = 2 : returnValue = 1;
	return returnValue;
}

void crackpw(Context *contexte) {
	
	Logger *logger = contexte->logger;
	std::string p_target_hash = contexte->hash;
	std::string p_algo = contexte->algo;
	std::string p_alphabet = contexte->alphabet;
	std::string p_chunksize = std::to_string(contexte->chunkSize);
	CPasswordChunk &p_repriseChunk = *contexte->repriseChunk;
	
	char password[64] = "";
	std::string alphabet = p_alphabet;
	std::string currentHash = "";
	
	Fifo<CPasswordChunk> *pwdFifo = contexte->fifo;
	Hasher *hasher;


	hasher = Hasher::getInstance();
	hasher->initialize(p_algo);
	logger->newMessage(1, "Lancement du programme.");

	CPasswordChunk pwdChunk;
	
	bool isResuming = (p_repriseChunk.GetChunkSize() == 0 ? false : true); // Est-on en train de reprendre une recherche interrompue ?
	bool passwordFound = false;				// Bool de Mot de passe trouvé
	bool isAborted = false;					// Bool de Arrêt avant mot de passe trouvé
	bool isRunning = false;					// Bool de Recherche en cours

	char lastLetter = alphabet.back();		// Dernière lettre de l'alphabet
	const int MAX_PWD_LENGTH = 10;			// Taille max du mot de passe à trouver. (Rajouter 2)
	int chunkSize = (atoi(p_chunksize.c_str()) - 2);	// -2 vu qu'on rajoute deux lettres à la fin du chunk quoi qu'il arrive.
	if (atoi(p_chunksize.c_str()) - 2 < 0) { int chunkSize = 0; }
	char currentChunkStart[MAX_PWD_LENGTH] = "";	// "Meta" Mot de passe pour incrémenter les chunks.

	// Définition : Un Meta Mot de passe est utilisé pour calculer les chunks. On rajoute "aa" ou "**", pour trouver le début / fin du chunk. Exemple : "hij" = "hijaa" -> "hij**"

	std::string firstPwd;					// Premier chunk à trouver, moins 1. On fera "+1" au début de la boucle tout à l'heure.
	for (int i = 1; i < chunkSize && i < MAX_PWD_LENGTH; i++) {	// "" donne un premier chunk de aaa -> a**
		firstPwd += lastLetter;			// "*" donne un premier chunk de "aaaa" -> "aa**", etc...		
	}

	strncpy_s(currentChunkStart, firstPwd.c_str(), _TRUNCATE);

	std::string pwdStartTemp;		// "Meta" Password de début de chunk.
	std::string pwdEndTemp;			// "Meta" Password de fin de chunk.

	// Procédure de reprise à partir d'un fichier de sauvegarde.
	if (isResuming) {
		// Déplacer le début du chunk de reprise (- les 2 derniers charactères) dans le currentChunkStart)
		strncpy_s(currentChunkStart, p_repriseChunk.GetPasswordBegin().substr(0, p_repriseChunk.GetPasswordBegin().size() - 2).c_str(), _TRUNCATE);

		// Déplacer le début du chunk de reprise et de fin dans leurs variables respectives
		pwdStartTemp = p_repriseChunk.GetPasswordBegin();
		pwdEndTemp = p_repriseChunk.GetPasswordEnd();
	}

	// Boucle Principale
	strcpy_s(password, sizeof(password), "");
	logger->newMessage(3, "Recherche en cours...");
	do {
		isRunning = true; 

		// Remplissage dela FIFO si taille moins de 3 chunks restants.
		if (pwdFifo->getSize() < 3) {
			// Boucle de remplissage de la FIFO
			while (pwdFifo->getSize() < 10) {
				pwdChunk.Reset();
				if (isResuming) {
					isResuming = false;
				}
				else {
					HashCrackerUtils::IncreasePassword(currentChunkStart, MAX_PWD_LENGTH, alphabet);
				}
				
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
			std::ofstream fichier("statut.txt", std::ios::out | std::ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
			if (fichier) {
				fichier << alphabet << std::endl;
				fichier << pwdChunk.GetPasswordBegin() << std::endl;
				fichier << p_algo << std::endl;
				fichier << p_target_hash << std::endl;
				fichier.close();
			}
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

			if (currentHash == p_target_hash) { //CRC32 : "884863D2" = 123 | "2D640152" = 900
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

	if (isAborted) {
		std::ofstream fichier("statut.txt", std::ios::out | std::ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
		if (fichier) {
			fichier << alphabet << std::endl;
			fichier << pwdChunk.GetPasswordBegin() << std::endl;
			fichier << p_algo << std::endl;
			fichier << p_target_hash << std::endl;
			fichier.close();
		}
	}
	else if (passwordFound) {
		std::ofstream fichier("statut.txt", std::ios::out | std::ios::trunc);
		fichier.close();
	}
}


int main( int argc, const char *argv[] ) {
	std::cout << "Projet Multithreading : Alois - Tristan - Jeremy" << std::endl;
	std::cout << std::endl;

	// Prêt à lancer
	bool isReadyToStart = false;

	// Le contexte contient toutes les informations globales de l'application (hash, algo, etc...)
	Context* contexte = new Context();;

	Logger *logger;
	logger = Logger::getInstance();
	logger->initialize(3);			// On indique ici le niveau de verbosité du logger.

	CPasswordChunk repriseChunk;

	// On vérifie si les arguments sont bon ou pas.
	// Ajoutez -forcenew pour ignorer le fichier de sauvegarde
	// ATTENTION : Entrainera l'écrasement de celui-ci
	if (ExtractCommandLine(argc, argv) == 1) {

		// Vérification de la présence possible d'une sauvegarde.
		std::string fileLine, startOldChunk, endOldChunk, alphabet, algo, hashCible;
		std::ifstream f("statut.txt");
		bool needsResume = false;
		int lineCounter = 1;
		while (std::getline(f, fileLine)) {
			switch (lineCounter) {
			case 1:
				alphabet = fileLine;
				logger->newMessage(1, "Alphabet : " + fileLine);
				break;
			case 2:
				startOldChunk = fileLine;
				endOldChunk = startOldChunk.substr(0, startOldChunk.size() - 2);
				endOldChunk = endOldChunk + alphabet.back() + alphabet.back();
				logger->newMessage(1, "Reprise de la recherche précédente interrompue au chunk : " + startOldChunk + " -> " + endOldChunk);
				needsResume = true;
				repriseChunk.SetPasswordRange(startOldChunk, endOldChunk);
				break;
			case 3:
				algo = fileLine;
				logger->newMessage(1, "Algo : " + fileLine);
				break;
			case 4:
				hashCible = fileLine;
				logger->newMessage(1, "Hash cible : " + fileLine);
				break;
			default:
				logger->newMessage(1, "Anomalie lors de la reprise d'un fichier de sauvegarde : Ligne en trop.");
				break;
			}
			lineCounter++;
		}

		if (needsResume) {
			logger->newMessage(1, "L'application a été précédemment interrompue, reprise...");
			contexte->fillContext(logger, hashCible, algo, alphabet, 0, "127.0.0.1", 0, &repriseChunk);
		}
		else {
			contexte->fillContext(logger, argv[2], argv[4], argv[6], atoi(argv[8]), "127.0.0.1", 0, &repriseChunk);
		}
		isReadyToStart = true;
	}
	else if (ExtractCommandLine(argc, argv) == 2) {
		contexte->fillContext(logger, argv[2], argv[4], argv[6], atoi(argv[8]), "127.0.0.1", 0, &repriseChunk);
		isReadyToStart = true;
	}
	else {
		std::cout << std::endl;
		std::cout << "Mauvais arguments de ligne de commande." << std::endl;

		// Display each command-line argument.
		std::cout << "Voici les arguments trouvés :" << std::endl;
		for (int count = 0; count < argc; count++) {
			std::cout << "  argv[" << count << "]   " << argv[count] << std::endl;
		}
		// Arguments à mettre : -hash 884863D2 -algo crc32 -alphabet abcdefghijklmnopqrstuvwxyz0123456789* -chunksize 3 
		// (optionnel -forcenew pour ignorer la reprise d'une sauvegarde)
	}

	if (isReadyToStart) {
		contexte->plugFifo(new Fifo<CPasswordChunk>());

		//Juste pour tester la création d'agents.
		LocalOrdonnancer* localOrdo = new LocalOrdonnancer(contexte);
		//localOrdo->putDownAgents();

		//crackpw(contexte);
	}
	
	std::cout << std::endl;
	std::cout << "Fermeture du programme." << std::endl;
	std::cin.get();


	return EXIT_SUCCESS;
}
