#include "AgentThread.h"
#include <iostream>

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

struct thread_args {
	AgentThread *instance;
};

void* thread_helper(void *voidArgs) {
	thread_args *args = (thread_args*)voidArgs;
	int res = args->instance->run(args->instance);
	return new int(res);
}

AgentThread::AgentThread(Context *_contexte)
{
	std::cout << "Nouveau AgentThread cree !" << std::endl;
	this->contexte = _contexte;

	thread_args args;
	args.instance = this;

	if (pthread_create(&this->thread, nullptr, &thread_helper, &args) != 0) {
		std::cerr << "** FAIL de creation du thread" << std::endl;
		return;
	}
}


void AgentThread::updateLastPassword()
{
	this->returnCode = 2;
	notify(this->returnCode, this->password);
}

void AgentThread::notifySuccess()
{
	std::cout << "AgentThread a trouve le mot de passe : " << this->password << std::endl;

	this->returnCode = 1;
	notify(this->returnCode, this->password);
}

void AgentThread::killAgent()
{
	this->returnCode = 0;
	// Et on notifie l'observateur
	notify(this->returnCode, this->password);

	// Le code 0 va automatiquement arrêter le thread
	std::cout << "AgentThread decede !" << std::endl;

}

int AgentThread::run(AgentThread *agent)
{
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
	Hasher *hasher;

	hasher = Hasher::getInstance();
	hasher->initialize(p_algo);
	logger->newMessage(1, "Lancement du thread.");

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
	} while (!passwordFound && !isAborted);
//
// Fin boucle principale
//

	return 0;
}


AgentThread::~AgentThread()
{

}
