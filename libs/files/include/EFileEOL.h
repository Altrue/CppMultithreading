#pragma once

typedef enum {
	none = 0,
	native = 1,
	UNIX = 2,
	Mac = 3,
	Windows = 4,

	LF = 2,
	CR = 3,
	CRLF = 4,
} EFileEOL;
