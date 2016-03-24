#include "Hasher.h"

#include "CException.h"

#include "IHash.h"
#include "CHashNone.h"
#include "CHashCrc32.h"
#include "CHashMd5.h"
#include "CHashSha1.h"
#include "CHashSha224.h"
#include "CHashSha256.h"
#include "CException.h"

// Public
Hasher::~Hasher()
{
	
}

Hasher::Hasher() {

}

void Hasher::initialize(std::string hashName) {

	if (hashName == "crc32") {
		this->iHasher = new CHashCrc32();
		//std::cout << "Hash en CRC32" << std::endl;
	}
	else if (hashName == "md5") {
		this->iHasher = new CHashMd5();
		//std::cout << "Hash en Md5" << std::endl;
	}
	else if (hashName == "sha1") {
		this->iHasher = new CHashSha1();
		//std::cout << "Hash en Sha1" << std::endl;
	}
	else if (hashName == "sha224") {
		this->iHasher = new CHashSha224();
		//std::cout << "Hash en Sha224" << std::endl;
	}
	else if (hashName == "sha256") {
		this->iHasher = new CHashSha256();
		//std::cout << "Hash en Sha256" << std::endl;
	}

	//std::cout << "On a le hasher" << std::endl;
}

std::string Hasher::calculateHash(const std::string p_text)
{
	try {
		unsigned char rawHash[128];
		iHasher->HashBuffer(reinterpret_cast<const unsigned char *>(p_text.c_str()), static_cast<int>(p_text.length()));
		iHasher->GetRawHash(&rawHash[0], sizeof(rawHash));
		return iHasher->GetHash();
	}
	catch (CException &e) {
		std::cerr << "** --- EXCEPTION THROWN ---" << std::endl;
		std::cerr << "** Type: " << e.GetType() << std::endl;
		std::cerr << "** Message: " << e.GetErrorMessage() << std::endl;
		std::cerr << "** Error code: " << e.GetErrorCode() << std::endl;
		std::cerr << "** Fault location: " << e.GetFaultLocation() << std::endl;
	}

	return ""; // Si on en arrive là, c'est un échec !
}
