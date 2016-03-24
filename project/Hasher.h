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

#include "singleton.h"

class Hasher
{
private:
	IHash *iHasher;
public:
	void initialize(std::string hashName);
	std::string calculateHash(const std::string p_text);
	~Hasher();
	Hasher();
};

