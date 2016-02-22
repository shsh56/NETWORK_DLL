// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the NETWORK_DLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// NETWORK_DLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef NETWORK_DLL_EXPORTS
#define NETWORK_DLL_API __declspec(dllexport)
#else
#define NETWORK_DLL_API __declspec(dllimport)
#endif
#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#include "targetver.h"
#include "string"
#include <stdio.h>
#include <tchar.h>
using namespace std;
#undef UNICODE
#include <stdlib.h>
#define DEFAULT_BUFLEN 512

// This class is exported from the NETWORK_DLL.dll
class NETWORK_DLL_API Network
{
private:
	struct addrinfo *result;
	struct addrinfo hints;

public:
	Network(void);
	~Network(void);
	SOCKET Connect(PCSTR ip, PCSTR port);
	SOCKET Bind(PCSTR ip, PCSTR port);
	SOCKET Listen(SOCKET ListenSocket);
	SOCKET Accept(SOCKET ListenSocket);
	void Send(SOCKET ConnectSocket, char sendbuf[DEFAULT_BUFLEN]);
	void Recv(SOCKET ConnectSocket, char recvbuf[DEFAULT_BUFLEN]);
};

extern NETWORK_DLL_API int nNETWORK_DLL;

NETWORK_DLL_API int fnNETWORK_DLL(void);
