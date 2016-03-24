#include "AgentThread.h"
#include <iostream>
#include <deque>
#include "pthread.h"

#include "utils.h"
#include "Hasher.h"
#include "IHash.h"
#include "CHashNone.h"
#include "CHashCrc32.h"
#include "CHashMd5.h"
#include "CHashSha1.h"
#include "CHashSha224.h"
#include "CHashSha256.h"
#include "CException.h"
#include "CUtil.h"

void AgentThread::updateLastPassword()
{
	this->returnCode = 1;
	notify(this->returnCode, this->password);
}

void AgentThread::notifySuccess()
{
	//std::cout << "AgentThread a trouve le mot de passe : " << this->password << std::endl;

	this->returnCode = 2;
	notify(this->returnCode, this->password);
}

void AgentThread::killAgent()
{
	this->returnCode = 0;
	// Et on notifie l'observateur
	notify(this->returnCode, this->password);

	// Le code 0 va automatiquement arrêter le thread
	contexte->logger->newMessage(2, "AgentThread decede !");

}

void *run(void *voidArgs)
{
	CUtil::Sleep(500); // On laisse le temps au local ordonnancer de se lancer proprement

	AgentThread *agent = (AgentThread *)voidArgs;
	Context *contexte = agent->contexte;

	// TODO : L'agent calcule des hash
	int saveCount = 0; // Compteur pour sauvegarder périodiquement

	Logger *logger = contexte->logger;
	std::string p_target_hash = contexte->hash;
	std::string p_algo = contexte->algo;
	std::string p_alphabet = contexte->alphabet;
	std::string p_chunksize = std::to_string(contexte->chunkSize);

	char password[64] = "";
	std::string alphabet = p_alphabet;
	std::string currentHash = "";

	Fifo<CPasswordChunk> *pwdFifo = contexte->fifo;
	Hasher hasher;

	hasher.initialize(p_algo);
	logger->newMessage(2, "Lancement du thread.");

	CPasswordChunk pwdChunk;

	bool passwordFound = false;				// Bool de Mot de passe trouvé
	bool isAborted = false;					// Bool de Arrêt avant mot de passe trouvé
	bool isRunning = false;					// Bool de Recherche en cours

//
// Boucle Principale
//

	strcpy_s(password, sizeof(password), "");
	do {
		isRunning = true;

		if (pwdFifo->getSize() > 0) {
				pwdChunk = pwdFifo->pull();
		
			strncpy_s(password, pwdChunk.GetPasswordBegin().c_str(), _TRUNCATE); // Conversion du string de début du chunk en char[]

		// Boucle de traitement d'un chunk
			logger->newMessage(4, "Debut lecture du chunk...");
			do {
				HashCrackerUtils::IncreasePassword(password, sizeof(password), alphabet);
				currentHash = hasher.calculateHash(password);

				//Décommentez cette ligne ci-dessous pour afficher les tentatives une à une :
				//std::cout << password << " -> " << currentHash << "" << std::endl;

				if (currentHash == p_target_hash) { //CRC32 : "884863D2" = 123 | "2D640152" = 900
					isRunning = false;
					passwordFound = true;
					agent->password = password;
					agent->notifySuccess();
				}

				if (std::string(password) == pwdChunk.GetPasswordEnd()) {
					isRunning = false;
					logger->newMessage(4, "...Fin lecture du chunk | Taille FIFO : " + std::to_string(pwdFifo->getSize()));
				}

				if (agent->returnCode == 0) {
					isRunning = false;
					isAborted = true;
				}
			} while (isRunning);
		// Fin boucle traitement chunk

			if (saveCount >= 10) {
				agent->password = pwdChunk.GetPasswordBegin();
				agent->updateLastPassword();
				saveCount = 0;
			}

			saveCount++;
		}
		else {
			// FIFO vide, attente.
			CUtil::Sleep(50);
		}
	} while (!passwordFound && !isAborted);
//
// Fin boucle principale
//

	return nullptr;
}


AgentThread::AgentThread(Context *_contexte)
{
	//std::cout << "Nouveau AgentThread cree !" << std::endl;
	this->contexte = _contexte;

	if (pthread_create(&this->thread, nullptr, &run, (void *)this) != 0) {
		contexte->logger->newMessage(0, "ECHEC de creation du thread");
		return;
	}
}

AgentThread::~AgentThread()
{

}
