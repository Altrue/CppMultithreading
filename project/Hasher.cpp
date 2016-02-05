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

// Private
Hasher::Hasher(std::string hashName)
{
	IHash *hasher = nullptr;

	if (hashName == "crc32") {
		hasher = new CHashCrc32();
		std::cout << "Hash en CRC32" << std::endl;
	}
	else if (hashName == "md5") {
		hasher = new CHashMd5();
		std::cout << "Hash en Md5" << std::endl;
	}
	else if (hashName == "sha1") {
		hasher = new CHashSha1();
		std::cout << "Hash en Sha1" << std::endl;
	}
	else if (hashName == "sha224") {
		hasher = new CHashSha224();
		std::cout << "Hash en Sha224" << std::endl;
	}
	else if (hashName == "sha256") {
		hasher = new CHashSha256();
		std::cout << "Hash en Sha256" << std::endl;
	}
}

// Public
Hasher::~Hasher()
{
}

Hasher Hasher::getInstance(std::string hashName)
{
	if (&instance == nullptr) {
		instance = Hasher(hashName);
		return instance;
	}
	else {
		return instance;
	}
}

std::string Hasher::calculateHash(const std::string p_text)
{
	try {
		unsigned char rawHash[128];
		hasher->HashBuffer(reinterpret_cast<const unsigned char *>(p_text.c_str()), static_cast<int>(p_text.length()));
		hasher->GetRawHash(&rawHash[0], sizeof(rawHash));
		return hasher->GetHash();
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
