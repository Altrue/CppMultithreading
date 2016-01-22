#pragma once

#include <iostream>
#include "IHash.h"

class CHashCrc32 : public IHash
{
private:
	std::string m_lastKnownHash;
	unsigned long m_lastKnownRawHash;
	unsigned long m_array_ulCRC32[ 256 ];

public:
	static std::string GetVersion();
	static std::string GetBuildDate();

	CHashCrc32();
	CHashCrc32( unsigned long p_polynom );
	~CHashCrc32();

	void HashFile(const std::string p_fileName);
	void HashBuffer(const unsigned char *p_buffer, const int p_bufferLength);

	int GetHashSize();
	void GetRawHash( void *p_targerBuffer4Bytes, const unsigned int p_targetBufferSize );
	std::string GetHash();
};
