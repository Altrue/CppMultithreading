#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "CFSObject.h"
#include "CFile.h"


class CFileBinary : public CFile {
public:
	static std::string GetVersion();
	static std::string GetBuildDate();

	CFileBinary();
	CFileBinary( const std::string &p_fileName );
	CFileBinary( const std::string &p_fileName, EFileOpenMode p_mode );
	~CFileBinary();

	void SetPos( off_t p_pos );
	unsigned int ReadData( unsigned int p_readByteCount, unsigned char *p_buffer, unsigned int p_bufferCapacity );
	unsigned int WriteData( const unsigned char *p_buffer, unsigned int p_length );
};
