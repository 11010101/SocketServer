#ifndef _SERVER_H_
#define _SERVER_H_

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <fstream>

#define print(...) \
		fprintf(stdout, __VA_ARGS__)

#if defined(_WIN32)

#include <WinSock2.h>
#include <WS2tcpip.h>

void Cleanup(SOCKET* socket) {
	int res;

	res = closesocket(*socket);
	if (res == SOCKET_ERROR) {
		print("Failed to close socket!\nErrorcode: %d", WSAGetLastError());
		WSACleanup();
		exit(1);
	}
}

#pragma comment(lib, "ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#elif defined(__LINUX__) || defined(__APPLE__)

#include <sys/socket.h>
#include <sys/types.h>

#endif

#endif // __SERVER_H__
