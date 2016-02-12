#pragma once

#include "stdafx.h"
#include <iostream>
#include <typeinfo>

#include "CException.h"

#include "IHash.h"
#include "CHashNone.h"
#include "CHashCrc32.h"
#include "CHashMd5.h"
#include "CHashSha1.h"
#include "CHashSha224.h"
#include "CHashSha256.h"
#include "CException.h"

class Hasher
{
private:
	static Hasher &instance;
	IHash *hasher;
	Hasher(std::string hashName);
public:
	static Hasher Hasher::getInstance(std::string hashName);
	std::string calculateHash(const std::string p_text);
	~Hasher();
};

