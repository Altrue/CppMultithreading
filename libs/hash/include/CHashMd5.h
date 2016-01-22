#pragma once

#include <iostream>
#include "IHash.h"

class CHashMd5 : public IHash
{
private:
	std::string m_lastKnownHash;
	unsigned char m_lastKnownRawHash[ 20 ];

public:
	static std::string GetVersion();
	static std::string GetBuildDate();

	CHashMd5();
	~CHashMd5();

	void HashFile( const std::string p_fileName );
	void HashBuffer( const unsigned char *p_buffer, const int p_bufferLength );

	int GetHashSize();
	std::string GetHash();
	void GetRawHash( void *p_targerBuffer16Bytes, const unsigned int p_targetBufferSize );
};
