#include <WinSock2.h>
#include <stdio.h>
#include <inttypes.h>  // TO print unit64_t

#pragma warning(disable:4996)  // To use inet_addr()
#pragma comment(lib, "ws2_32.lib")

#define PORT 9000
#define SIZE 10
#define DATATYPE uint64_t

#define MAX_BUF_LEN 20
#define DATA_LEN 10


void error_handling(char* msg);

struct socketFrame {
	DATATYPE data[SIZE];
};

int main()
{
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddr, clientAddr;

	// Response DATA: configurable
	struct socketFrame socketData;
	int i;
	for (i = 0; i < DATA_LEN; i++) {
		socketData.data[i] = i;
	}

	int maxClientNum = 2;  // configurable: The maximum number of clients to connect
	char ping[MAX_BUF_LEN];
	int nRecv;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		error_handling("Load Winsock 2.2 Error");

	serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET)
		error_handling("Socket Error");

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(PORT);

	if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		error_handling("Connection Error");

	if (listen(serverSocket, maxClientNum) == SOCKET_ERROR)
		error_handling("Connection Error");

	int size = sizeof(clientAddr);
	clientSocket = accept(serverSocket, (void*)&clientAddr, &size);
	if (clientSocket == SOCKET_ERROR)
		error_handling("Connection Error");

	nRecv = recv(clientSocket, ping, sizeof(ping) - 1, 0);
	if (nRecv == SOCKET_ERROR)
		error_handling("Receive Error");
	ping[nRecv] = '\0';
	printf("Receive message: %s\n", ping);
	
	send(clientSocket, (void*)&socketData, sizeof(socketData), 0);

	closesocket(serverSocket);
	closesocket(clientSocket);
	WSACleanup();

	return 0;
}

void error_handling(char* msg) {
	// 프로젝트 -> 속성(맨밑창) -> 문자 집합 -> '멀티바이트 문자 집합 사용'
	MessageBox(NULL, msg, "Error", MB_OK);
	exit(1);
}