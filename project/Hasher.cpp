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

Hasher::Hasher()
{

}

Hasher::~Hasher()
{

}

std::string calculateHash(IHash *p_hasher, const std::string p_text) {
	try {
		unsigned char rawHash[128];
		p_hasher->HashBuffer(reinterpret_cast<const unsigned char *>(p_text.c_str()), static_cast<int>(p_text.length()));
		p_hasher->GetRawHash(&rawHash[0], sizeof(rawHash));
		return p_hasher->GetHash();
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