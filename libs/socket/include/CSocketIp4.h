#pragma once

#include "CSocket.h"


class CSocketIp4 : public CSocket {
public:
	// Miscellaneous operations about local IP addresses
	bool IsIpAddressPrivate( const char *p_ip );
	std::vector<std::string> GetLocalIpAddresses();
	std::vector<std::string> DnsResolve( const char *p_nameToResolve );
	std::string GetLocalEndpointIp();
	unsigned short GetLocalEndpointPort();

	// TCP operations
	void Connect( const char *p_remoteAddress, unsigned short p_remotePort, unsigned int p_milliseconds );
	CSocket *Accept();

	// TCP server operations
	void CreateServer( unsigned short p_localPort, unsigned short p_maxConnections, int p_socketType = SOCK_STREAM, int p_protocol = IPPROTO_TCP, const char *p_ipToBind = "" );
};
