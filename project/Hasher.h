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
public:
	Hasher();
	~Hasher();
};

std::string calculateHash(IHash *p_hasher, const std::string p_text);

