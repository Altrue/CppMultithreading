#pragma once

#include <iostream>
#include "IHash.h"

class CHashNone : public IHash
{
private:
	std::string m_lastKnownHash;
	unsigned char m_lastKnownRawHash[ 32 ];

public:
	static std::string GetVersion();
	static std::string GetBuildDate();

	CHashNone();
	~CHashNone();

	void HashFile( const std::string p_fileName );
	void HashBuffer( const unsigned char *p_buffer, const int p_bufferLength );

	int GetHashSize();
	std::string GetHash();
	void GetRawHash( void *p_targerBuffer32Bytes, const unsigned int p_targetBufferSize );
};
