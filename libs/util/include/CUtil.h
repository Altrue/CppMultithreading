#pragma once

#include <iostream>
#include <string>

class CUtil {
public:
	static std::string GetVersion();
	static std::string GetBuildDate();

	static bool IsEscKeyPressed();
	static void WaitForEscKeyPressed();
	static int GetCpuCoreCount();
	static void Sleep( int p_millisec );
};
