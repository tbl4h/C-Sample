#include <cstdio>
#include <cstdlib>

#include <winsock2.h>
#include <Ws2tcpip.h>
int main()
{
	WSADATA wsaData;

	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != NO_ERROR)
		printf("Initialization error.\n");
	SOCKET mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mainSocket == INVALID_SOCKET)
	{
		printf("Error creating socket: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	sockaddr_in service;
	memset(&service, 0, sizeof(service));
	service.sin_family = AF_INET;
	inet_pton(AF_INET, (PCSTR)"127.0.0.1", &service.sin_addr.s_addr);
	service.sin_port = htons(27015);
	if (connect(mainSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		printf("Failed to connect.\n");
		WSACleanup();
		return 1;
	}
	int bytesSent;
	int bytesRecv = SOCKET_ERROR;
	char sendbuf[32] = "Client says hello!";
	char recvbuf[32] = "";

	bytesSent = send(mainSocket, sendbuf, strlen(sendbuf), 0);
	printf("Bytes sent: %ld\n", bytesSent);

	while (bytesRecv == SOCKET_ERROR)
	{
		bytesRecv = recv(mainSocket, recvbuf, 32, 0);

		if (bytesRecv == 0 || bytesRecv == WSAECONNRESET)
		{
			printf("Connection closed.\n");
			break;
		}

		if (bytesRecv < 0)
			return 1;

		printf("Bytes received: %ld\n", bytesRecv);
		printf("Received text: %s\n", recvbuf);
	}

	system("pause");

}
