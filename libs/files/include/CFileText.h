#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "CFSObject.h"
#include "CFile.h"


class CFileText : public CFile {
public:
	static std::string GetVersion();
	static std::string GetBuildDate();

	CFileText();
	CFileText( const std::string &p_fileName );
	CFileText( const std::string &p_fileName, EFileOpenMode p_mode );
	~CFileText();

	std::vector<std::string> ReadAll();
	void WriteAll( const std::vector<std::string> &p_lines, EFileEOL p_eolType = EFileEOL::native );

	std::string ReadNextLine();
	void AppendLine( const std::string &p_line, EFileEOL p_eoType = EFileEOL::native );
};
