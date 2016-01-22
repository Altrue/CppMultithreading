#include "stdafx.h"
#include <iostream>
#include "DemoUtil.h"

#include "CUtil.h"

void TestUtil() {
	std::cout << "--- Util library tests ---" << std::endl;

	std::cout << "CPU core count on this computer: " << CUtil::GetCpuCoreCount() << std::endl;

	std::cout << "Now testing keyboard. Press <ESC> key to continue";
	while (!CUtil::IsEscKeyPressed()) {
		std::cout << ".";
		CUtil::Sleep( 50 );
	}
	std::cout << std::endl << "Okay!" << std::endl;

	std::cout << "Now waiting for <ESC> key to be pressed.";
	CUtil::WaitForEscKeyPressed();
	std::cout << "Okay!" << std::endl;

	std::cout << "--- End of tests ---" << std::endl;
	return;
}
