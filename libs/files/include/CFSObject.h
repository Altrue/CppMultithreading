#pragma once

#include <iostream>
#include "CDateTime.h"


class CFSObject {
protected:
	std::string m_name;
	CDateTime m_created, m_modified, m_accessed;

public:
	static std::string GetVersion();
	static std::string GetBuildDate();

	CFSObject();
	CFSObject( const std::string &p_name );
	virtual ~CFSObject();

/*
	std::string getFullName();
	std::string getDirName();
	std::string getBaseName();
	std::string getExtension();

	void refresh();

	bool exist();

	bool isDirectory();
	bool isRegular();
	bool isLink();

	bool isAttribReadOnly();
	bool isAttribSystem();
	bool isAttribHidden();
	bool isAttribArchive();
	bool isAttribCompressed();

	void setAttribReadOnly();
	void setAttribSystem();
	void setAttribHidden();
	void setAttribArchive();
	void setAttribCompressed();

	CDateTime &getDateCreated();
	CDateTime &getDateModified();
	CDateTime &getDateAccessed();
*/
};
