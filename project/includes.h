#pragma once

#include "stdafx.h"
#include "utils.h"

#include <vector>
#include <list>
#include <iostream>
#include <string>
#include <deque>
#include <windows.h>

#include "IHash.h"
#include "CHashNone.h"
#include "CHashCrc32.h"
#include "CHashMd5.h"
#include "CHashSha1.h"
#include "CHashSha224.h"
#include "CHashSha256.h"
#include "CException.h"
#include "CUtil.h"

#include "AgentThread.h"
#include "Fifo.h"
#include "Fifo.cpp"
#include "Hasher.h"
#include "LocalOrdonnancer.h"
#include "Logger.h"
#include "PasswordChunk.h"
#include "Observable.h"
#include "Observer.h"
#include "start.h"

// WOODOO BULLSHIT