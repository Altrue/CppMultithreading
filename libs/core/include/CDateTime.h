#pragma once

#include <iostream>


// Let compiler know some definitions when outside from the Windows world
#if !defined(_INC_WINDOWS)
typedef unsigned short	WORD;
typedef unsigned long	DWORD;

typedef struct _FILETIME {
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
} FILETIME, *PFILETIME;
#endif

#if !defined(_WIN32) && !defined(_WIN64)
#include <inttypes.h>
typedef int64_t __int64;
#endif


#pragma pack(push, 1)

class CDateTime {
public:
	// This structure is very similar to SYSTEMTIME from Windows
	WORD m_wYear;
	WORD m_wMonth;
	WORD m_wDayOfWeek;
	WORD m_wDay;
	WORD m_wHour;
	WORD m_wMinute;
	WORD m_wSecond;
	WORD m_wMilliseconds;

public:
	static std::string GetVersion();
	static std::string GetBuildDate();

	CDateTime();
	CDateTime( int p_year, int p_month, int p_day );
	CDateTime( int p_hour, int p_minute, int p_second, int p_millisecond );
	CDateTime( int p_year, int p_month, int p_day, int p_hour, int p_minute, int p_second, int p_millisecond );
	explicit CDateTime( const FILETIME &p_ftime, bool p_gmt = false );
	explicit CDateTime( const time_t &p_time, bool p_gmt = false );
	~CDateTime();

	time_t ToTimeT();
	FILETIME ToFileTime();

	void FromTimeT( const time_t &p_time, bool p_gmt = false );
	void FromFileTime( const FILETIME &p_time, bool p_gmt = false );

	bool operator == ( const CDateTime &p_other ) const;
	bool operator < ( const CDateTime &p_other ) const;
	bool operator > ( const CDateTime &p_other ) const;

	void Now( bool p_gmt = false );
	void Today( bool p_gmt = false );
	void Time( bool p_gmt = false );

	void Reset();
	void ResetDate();
	void ResetTime();
	bool IsNull();

	int GetTimeZoneOffset();
	std::string ToString();
};

#pragma pack(pop)
