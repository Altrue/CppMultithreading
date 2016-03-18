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

LocalOrdonnancer::LocalOrdonnancer(Context *_context)
{
	createAgents(_context);
}


LocalOrdonnancer::~LocalOrdonnancer()
{

}

void LocalOrdonnancer::createAgents(Context *_context)
{
	for (int n = 0; n < this->coreCount; n++) {
		AgentThread* thread = new AgentThread(_context);
		thread->attach(this);
		std::cout << "AgentThread " << n << " est desormais observe !" << std::endl;
		this->vectorAgents.push_back(thread);
	}

	run(_context); // Eeeett c'est parti !
}

void LocalOrdonnancer::putDownAgents()
{
	for (int n = 0; n < this->coreCount; n++) {
		this->vectorAgents[n]->killAgent();
	}

	this->vectorAgents.clear();
}

void LocalOrdonnancer::update(int returnCode, std::string returnPassword)
{
	// Mis à jour car un sujet a une info à faire parvenir
	if (returnCode == 0) {
		std::cout << "L'observateur a ete notifie de la mort d'un AgentThread" << std::endl;
	}
	else if (returnCode == 1) {
		// Mise à jour de l'avancement pour que la sauvegarde soit efficace.
		this->lastCheckedPassword = returnPassword;
	}
	else if (returnCode == 2) {
		std::cout << "L'observateur sait que le mot de passe a ete trouve : " << returnPassword << std::endl;
		this->putDownAgents(); // On arrête donc les agents.
		this->pwdFound = true; // Important pour arrêter la boucle run.
		this->password = returnPassword;
		// TODO : Transmettre la bonne nouvelle (et returnPassword) à l'ordonanceur global
	}
	else if (returnCode == -1) {
		std::cout << "L'observateur a ete notifie d'un message mais le dev a oublie de changer le returnCode donc on ne sait pas ce que c'est..." << std::endl;
	}
	else {
		std::cout << "L'observateur a ete notifie d'un message mais le dev a invente un nouveau returnCode qui n'existe pas..." << std::endl;
	}
	
}


void LocalOrdonnancer::run(Context *contexte)
{
	// TODO : Boucle de ce que fait le local ordo. Remplir la fifo et companie.

	this->pwdFound = false;

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

//
// Boucle Principale
//
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
				fichier << this->lastCheckedPassword << std::endl;
				fichier << p_algo << std::endl;
				fichier << p_target_hash << std::endl;
				fichier.close();
			}
			
			this->putDownAgents(); // On interrompt les threads.
		}
		
		if (GetAsyncKeyState(VK_ESCAPE) != 0) {
			isRunning = false;
			isAborted = true;
			logger->newMessage(0, "Arret demande par l'utilisateur");
		}

		if (this->pwdFound) {
			// On check régulièrement si ce bool n'est pas true.
			// Ce bool sera mis à jour si on a trouvé le truc.
			passwordFound = true;
			logger->newMessage(0, "Trouve ! Le mot de passe est : " + this->password);
		}

		CUtil::Sleep(50); // Dodo pendant 50ms

	} while (!passwordFound && !isAborted);
//
// Fin boucle principale
//

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


