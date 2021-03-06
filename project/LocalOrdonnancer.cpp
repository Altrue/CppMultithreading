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
	this->contexte = _context;
	this->pwdFound = false;
	createAgents();
}


LocalOrdonnancer::~LocalOrdonnancer()
{

}

void LocalOrdonnancer::createAgents()
{
	for (int n = 0; n < this->coreCount; n++) {
		AgentThread* thread = new AgentThread(contexte);
		thread->attach(this);
		//std::cout << "AgentThread " << n << " est desormais observe !" << std::endl;
		this->vectorAgents.push_back(thread);
	}

	run();
}

void LocalOrdonnancer::putDownAgents()
{
	for (int n = 0; n < this->vectorAgents.size(); n++) {
		this->vectorAgents[n]->killAgent();
	}

	this->vectorAgents.clear();
}

void LocalOrdonnancer::update(int returnCode, std::string returnPassword)
{
	// Mis � jour car un sujet a une info � faire parvenir
	if (returnCode == 0) {
		contexte->logger->newMessage(4, "L'observateur a ete notifie de la mort d'un AgentThread");
	}
	else if (returnCode == 1) {
		contexte->logger->newMessage(1, "Trouve ! Le mot de passe est : " + returnPassword);
		this->putDownAgents(); // On arr�te donc les agents.
		this->pwdFound = true; // Important pour arr�ter la boucle run.
		this->password = returnPassword;
		// TODO : Transmettre la bonne nouvelle (et returnPassword) � l'ordonanceur global
	}
	else if (returnCode == -1) {
		contexte->logger->newMessage(0, "L'observateur a ete notifie d'un message mais le dev a oublie de changer le returnCode donc on ne sait pas ce que c'est...");
	}
	else {
		contexte->logger->newMessage(0, "L'observateur a ete notifie d'un message mais le dev a invente un nouveau returnCode qui n'existe pas...");
	}
	
}


void LocalOrdonnancer::run()
{
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

	logger->newMessage(1, "Lancement du local ordonnancer.");

	CPasswordChunk pwdChunk;

	bool isResuming = (p_repriseChunk.GetChunkSize() == 0 ? false : true); // Est-on en train de reprendre une recherche interrompue ?
	bool isAborted = false;					// Bool de Arr�t avant mot de passe trouv�
	bool isRunning = false;					// Bool de Recherche en cours

	char lastLetter = alphabet.back();		// Derni�re lettre de l'alphabet
	const int MAX_PWD_LENGTH = 10;			// Taille max du mot de passe � trouver. (Rajouter 2)
	int chunkSize = (atoi(p_chunksize.c_str()) - 2);	// -2 vu qu'on rajoute deux lettres � la fin du chunk quoi qu'il arrive.
	if (atoi(p_chunksize.c_str()) - 2 < 0) { int chunkSize = 0; }
	char currentChunkStart[MAX_PWD_LENGTH] = "";	// "Meta" Mot de passe pour incr�menter les chunks.
													// D�finition : Un Meta Mot de passe est utilis� pour calculer les chunks.
													// On rajoute "aa" ou "**", pour trouver le d�but / fin du chunk. Exemple : "hij" = "hijaa" -> "hij**"
	
	// Pour sauvegarder la progression :
	// On prends le premier chunk de l'**avant-dernier** remplissage de la FIFO.
	// Cela garantit facilement l'exhaustivit� de la recherche en cas d'interruption, le gaspillage de temps est n�gligeable (quelques ms)
	std::string saveChunk = "";


	std::string firstPwd;											// Premier chunk � trouver, moins 1. On fera "+1" au d�but de la boucle tout � l'heure.
	for (int i = 1; i < chunkSize && i < MAX_PWD_LENGTH; i++) {		// Exemple 1 : "" donne un premier chunk de aaa -> a**
		firstPwd += lastLetter;										// Exemple 2 : "*" donne un premier chunk de "aaaa" -> "aa**"
	}

	strncpy_s(currentChunkStart, firstPwd.c_str(), _TRUNCATE);

	std::string pwdStartTemp;		// "Meta" Password de d�but de chunk. (c'est � dire sans les deux derniers chars qui sont toujours aa et **)
	std::string pwdEndTemp;			// "Meta" Password de fin de chunk.

									// Proc�dure de reprise � partir d'un fichier de sauvegarde.
	if (isResuming) {
		// D�placer le d�but du chunk de reprise (- les 2 derniers charact�res) dans le currentChunkStart
		strncpy_s(currentChunkStart, p_repriseChunk.GetPasswordBegin().substr(0, p_repriseChunk.GetPasswordBegin().size() - 2).c_str(), _TRUNCATE);

		// D�placer le d�but du chunk de reprise et de fin dans leurs variables respectives
		pwdStartTemp = p_repriseChunk.GetPasswordBegin();
		pwdEndTemp = p_repriseChunk.GetPasswordEnd();
	}

//
// Boucle Principale
//
	strcpy_s(password, sizeof(password), "");
	do {
		isRunning = true;

		// Remplissage de la FIFO si taille de moins de 5 chunks restants.
		// Note :	Il serait tr�s facile de faire une FIFO de taille proportionnelle au nombre de coeurs CPU.
		//			Mais cela ne permet que de passer d'environ 95% d'usage � ~100% d'usage CPU.
		//			La d�gradation de l'exp�rience utilisateur pendant une recherche est, en revanche, infiniment plus grande...
		if (pwdFifo->getSize() < 5) {
			// Boucle de remplissage de la FIFO
			pwdStartTemp = currentChunkStart;
			pwdStartTemp = pwdStartTemp + "aa";
			if (pwdStartTemp == "aa") {
				pwdStartTemp = "aaa";
			}
			std::cout << "Etat d'avancement : " << pwdStartTemp << std::endl; // Seule impression console non-logg�e du programme
			
			this->lastCheckedPassword = saveChunk;	// Modification de ce qu'est � mettre dans le fichier de sauvegarde si on presse ESC.
			saveChunk = pwdStartTemp; // On modifie la valeur de saveChunk **apr�s**, pour que cela n'ait d'impact sur lastCheckedPassword que lors du prochain passage.

			logger->newMessage(4, "Debut remplissage FIFO | Taille FIFO : " + std::to_string(pwdFifo->getSize()));
			while (pwdFifo->getSize() < 20) {
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
				logger->newMessage(4, "Nouveau Chunk Injecte : " + pwdStartTemp + " -> " + pwdEndTemp);
				
				if (GetAsyncKeyState(VK_ESCAPE) != 0) {
					isAborted = true;
					logger->newMessage(0, "Arret demande par l'utilisateur");
					break;
				}
			} // Fin boucle remplissage FIFO
			logger->newMessage(4, "Fifo remplie | Taille FIFO : " + std::to_string(pwdFifo->getSize()));
		}

		// Tr�s tr�s rare d'arriver � abort pendant le (tr�s rapide) remplissage d'une FIFO.
		if (isAborted) {
			if (this->lastCheckedPassword != "") {
				std::ofstream fichier("statut.txt", std::ios::out | std::ios::trunc);  // ouverture en �criture avec effacement du fichier ouvert
				if (fichier) {
					fichier << alphabet << std::endl;
					fichier << this->lastCheckedPassword << std::endl;
					fichier << p_algo << std::endl;
					fichier << p_target_hash << std::endl;
					fichier.close();

					logger->newMessage(3, "Dernier chunk sauvegarde : " + this->lastCheckedPassword);
				}
			}
			
			this->putDownAgents(); // On interrompt les threads.
		}
		
		if (GetAsyncKeyState(VK_ESCAPE) != 0) {
			isRunning = false;
			isAborted = true;
			logger->newMessage(0, "Arret demande par l'utilisateur");
		}

		CUtil::Sleep(50); // Pause pendant 50ms avant de re-v�rifier l'�tat de la FIFO.

	} while (!this->pwdFound && !isAborted);
//
// Fin boucle principale
//

	if (isAborted) {
		if (this->lastCheckedPassword != "") {
			std::ofstream fichier("statut.txt", std::ios::out | std::ios::trunc);  // ouverture en �criture avec effacement du fichier ouvert
			if (fichier) {
				fichier << alphabet << std::endl;
				fichier << this->lastCheckedPassword << std::endl;
				fichier << p_algo << std::endl;
				fichier << p_target_hash << std::endl;
				fichier.close();

				logger->newMessage(3, "Dernier chunk sauvegarde : " + this->lastCheckedPassword);
			}
		}

		this->putDownAgents(); // On interrompt les threads.
	}
	else if (this->pwdFound) {
		std::ofstream fichier("statut.txt", std::ios::out | std::ios::trunc);
		fichier.close();
	}
}


