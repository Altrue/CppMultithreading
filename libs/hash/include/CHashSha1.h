#pragma once

#include <iostream>
#include "IHash.h"

class CHashSha1 : public IHash
{
private:
	std::string m_lastKnownHash;
	unsigned char m_lastKnownRawHash[ 20 ];

public:
	static std::string GetVersion();
	static std::string GetBuildDate();

	CHashSha1();
	~CHashSha1();

	void HashFile( const std::string p_fileName );
	void HashBuffer( const unsigned char *p_buffer, const int p_bufferLength );

	int GetHashSize();
	std::string GetHash();
	void GetRawHash( void *p_targerBuffer20Bytes, const unsigned int p_targetBufferSize );
};
