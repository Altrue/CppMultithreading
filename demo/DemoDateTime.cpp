#include "stdafx.h"
#include <iostream>
#include <string>
#include "DemoDateTime.h"

#include "CDateTime.h"


void TestDateTime() {
	std::cout << std::endl;
	std::cout << "--------------------------------------------------" << std::endl;

	std::cout << "Starting date-time library tests" << std::endl;
	std::cout << std::endl;

	std::cout << "Using datetime library v" << CDateTime::GetVersion() << " as of " << CDateTime::GetBuildDate() << "." << std::endl;


	CDateTime now, d;

	// Now
	std::cout << std::endl;
	std::cout << "Getting now..." << std::endl;
	now.Now();
	std::cout << "Now is: " << now.m_wYear << "-" << now.m_wMonth << "-" << now.m_wDay << " @ " << now.m_wHour << ":" << now.m_wMinute << ":" << now.m_wSecond << "." << now.m_wMilliseconds << std::endl;

	// Copy
	std::cout << std::endl;
	std::cout << "Copying from now..." << std::endl;
	d = now;
	std::cout << "d is: " << d.m_wYear << "-" << d.m_wMonth << "-" << d.m_wDay << " @ " << d.m_wHour << ":" << d.m_wMinute << ":" << d.m_wSecond << "." << d.m_wMilliseconds << std::endl;
	if (d < now) {
		std::cout << "d is before now." << std::endl;
	}
	else if (d > now) {
		std::cout << "d is after now." << std::endl;
	}
	else {
		std::cout << "d is same time as now." << std::endl;
	}

	// Clear time part
	std::cout << std::endl;
	std::cout << "Reseting time part..." << std::endl;
	d = now;
	d.ResetTime();
	std::cout << "d is: " << d.m_wYear << "-" << d.m_wMonth << "-" << d.m_wDay << " @ " << d.m_wHour << ":" << d.m_wMinute << ":" << d.m_wSecond << "." << d.m_wMilliseconds << std::endl;

	if (d < now) {
		std::cout << "d is before now." << std::endl;
	}
	else if (d > now) {
		std::cout << "d is after now." << std::endl;
	}
	else {
		std::cout << "d is same time as now." << std::endl;
	}

	// Clear date part
	std::cout << std::endl;
	std::cout << "Reseting time part..." << std::endl;
	d = now;
	d.ResetDate();
	std::cout << "d is: " << d.m_wYear << "-" << d.m_wMonth << "-" << d.m_wDay << " @ " << d.m_wHour << ":" << d.m_wMinute << ":" << d.m_wSecond << "." << d.m_wMilliseconds << std::endl;

	if (d < now) {
		std::cout << "d is before now." << std::endl;
	}
	else if (d > now) {
		std::cout << "d is after now." << std::endl;
	}
	else {
		std::cout << "d is same time as now." << std::endl;
	}

	std::cout << std::endl;
	return;
}
