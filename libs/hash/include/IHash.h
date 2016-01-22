#pragma once

// Interface to hash algorithms
#include <iostream>

class IHash
{
public:
	static std::string GetVersion();
	static std::string GetBuildDate();

	IHash() {};
	virtual ~IHash() {};

	// "=0" makes this method as "pure virtual", and also makes this class abstract
	virtual void HashFile(const std::string p_fileName) = 0;
	virtual void HashBuffer(const unsigned char *p_buffer, const int p_bufferLength) = 0;
	virtual int GetHashSize() = 0;
	virtual std::string GetHash() = 0;
	virtual void GetRawHash( void *p_targerBuffer, const unsigned int p_targetBufferSize ) = 0;
};
