#pragma once

#include <iostream>
#include "CFSObject.h"
#include "EFileEOL.h"
#include "EFileOpenMode.h"

class CFile : public CFSObject {
protected:
	std::fstream m_handle;

public:
	static std::string GetVersion();
	static std::string GetBuildDate();

	CFile();
	CFile( const std::string &p_fileName );
	CFile( const std::string &p_fileName, EFileOpenMode p_mode );
	~CFile();

	unsigned __int64 GetFileSize();
	void Remove() const;

	void Open( EFileOpenMode p_mode = EFileOpenMode::read );
	void Rewind();
	void Close();
	void Flush();
};
