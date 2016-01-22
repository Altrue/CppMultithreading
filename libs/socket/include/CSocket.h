#pragma once

#include <iostream>
#include <vector>


#if defined(_WIN32) || defined(_WIN64)
#if defined(_INC_WINDOWS)
#pragma message("NOTE: Including winsock.h instead of winsock2.h because windows.h is already included ...")
#include <winsock.h>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#endif
#else
typedef int SOCKET;
#endif


#include "CBrokenSocketException.h"
#include "CConnectionException.h"
#include "CDnsResolutionException.h"
#include "CServerSocketException.h"
#include "CSocketIOException.h"



#ifndef NO_TIMEOUT
#define NO_TIMEOUT                0xFFFFFFFF
#endif

#ifndef SOCKET_TIMEOUT
#define SOCKET_TIMEOUT            (-2)
#endif



class CSocket {
protected:
	SOCKET m_socket;
	std::string m_remoteIP;
	unsigned short m_remotePort;

public:
	CSocket();
	virtual ~CSocket();

	// Information about library
	static std::string GetVersion();
	static std::string GetBuildDate();

	// Initializations / un-initializations
	static void InitEngine();
	static void UninitEngine();

	// Socket type and protocol
	void SetSocketProto( int p_socketType = SOCK_STREAM, int p_protocol = IPPROTO_TCP );

	// Miscellaneous operations about local IP addresses
	virtual bool IsIpAddressPrivate( const char *p_ip ) = 0;
	virtual std::vector<std::string> GetLocalIpAddresses() = 0;
	virtual std::vector<std::string> DnsResolve( const char *p_nameToResolve ) = 0;
	virtual std::string GetLocalEndpointIp() = 0;
	virtual unsigned short GetLocalEndpointPort() = 0;

	// Miscellaneous operations about remote IP addresses
	std::string GetRemoteEndpointIp();
	unsigned short GetRemoteEndpointPort();

	// TCP operations
	virtual void Connect( const char *p_remoteAddress, unsigned short p_remotePort, unsigned int p_milliseconds ) = 0;
	unsigned int Send( const void* p_buffer, unsigned short p_byteCount, unsigned int p_milliseconds );
	unsigned int Recv( void* p_buffer, unsigned short p_bufferCapacity, unsigned int p_milliseconds );
	unsigned int Peek( void* p_buffer, unsigned short p_bufferCapacity, unsigned int p_milliseconds );
	void Shutdown( unsigned int p_milliseconds = 0 );
	void Close();

	// TCP server operations
	virtual void CreateServer( unsigned short p_localPort, unsigned short p_maxConnections, int p_socketType = SOCK_STREAM, int p_protocol = IPPROTO_TCP, const char *p_ipToBind = "" ) = 0;
	virtual CSocket *Accept() = 0;

	// Miscellaneous operations about socket
	bool IsListening();
	bool IsLastAsyncOperationPending();
	unsigned int GetReadyByteCount();

	// I/O operations
	void SetBlockingMode( bool p_blocking );
	bool IsErroneous();

	// Wait operations
	int WaitForRead( unsigned int p_milliseconds );
	int WaitForWrite( unsigned int p_milliseconds );

protected:
	void PrepareTimeval( struct timeval &tv, unsigned int p_milliseconds );
};
