// NETWORK_DLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "NETWORK_DLL.h"


// This is an example of an exported variable
NETWORK_DLL_API int nNETWORK_DLL=0;

// This is an example of an exported function.
NETWORK_DLL_API int fnNETWORK_DLL(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see NETWORK_DLL.h for the class definition
Network::Network(void)
{
	WSADATA wsaData;
	int iResult;
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
	}

	ZeroMemory(&this->hints, sizeof(this->hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_ALL;

}
SOCKET Network::Connect(PCSTR ip, PCSTR port)
{

	struct addrinfo *ptr;
	SOCKET ConnectSocket;
	int iResult = getaddrinfo(ip, port, &this->hints, &this->result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	// Create a SOCKET for connecting to server
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(this->result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}
	return ConnectSocket;

}
SOCKET Network::Bind(PCSTR ip, PCSTR port)
{
	int iResult = getaddrinfo(ip, port, &this->hints, &(this->result));
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
	}
	SOCKET ListenSocket = socket(this->result->ai_family, this->result->ai_socktype, this->result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
	}
	iResult = bind(ListenSocket, this->result->ai_addr, (int)this->result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
	}
	freeaddrinfo(this->result);
	return ListenSocket;
}
SOCKET Network::Listen(SOCKET ListenSocket)
{
	int iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
	}
	return ListenSocket;
}
SOCKET Network::Accept(SOCKET ListenSocket)
{
	// Accept a client socket
	SOCKET ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
	}

	// No longer need server socket
	closesocket(ListenSocket);
	return ClientSocket;
}
void Network::Send(SOCKET ConnectSocket, char sendbuf[DEFAULT_BUFLEN])
{
	int iResult = send(ConnectSocket, sendbuf, DEFAULT_BUFLEN, 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
	}
}
void Network::Recv(SOCKET ConnectSocket, char recvbuf[DEFAULT_BUFLEN])
{
	int iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0);
	if (iResult > 0)
		printf("Bytes received: %d\n", iResult);
	else if (iResult == 0)
		printf("Connection closed\n");
	else
		printf("recv failed with error: %d\n", WSAGetLastError());

}
Network::~Network(void)
{
}
