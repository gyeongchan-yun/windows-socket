#include <WinSock2.h>
#include <stdio.h>
#include <inttypes.h>  // To print unit64_t

#pragma warning(disable:4996)  // To use inet_addr()
#pragma comment(lib, "ws2_32.lib")


#define IP "127.0.0.1" 
#define PORT 9000
#define SIZE 10
#define DATATYPE uint64_t

#define MAX_BUF_LEN 2048

void error_handling(char* msg);

struct socketFrame {
	DATATYPE data[SIZE];
};

int main()
{
	WSADATA wsaData;
	SOCKET clientSocket;
	SOCKADDR_IN serverAddr;

	char data[MAX_BUF_LEN];
	struct socketFrame *socketData;
	int nRecv;
	int size = 0;
	int i;
	DATATYPE value;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		error_handling("Load Winsock 2.2 Error");

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
		error_handling("Socket Error");

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(IP);
	serverAddr.sin_port = htons(PORT);

	if (connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		error_handling("Connection Error");

	char ping[] = "client request";
	send(clientSocket, ping, sizeof(ping) - 1, 0);

	nRecv = recv(clientSocket, data, sizeof(data) - 1, 0);

	if (nRecv == SOCKET_ERROR)
		error_handling("Receive Error");
	data[nRecv] = '\0';
	socketData = (struct socketFrame*)data;
	for(i=0;i<SIZE;i++) {
		value = socketData->data[i];
		printf("%"PRIu64"\n", value);
	}
	
	closesocket(clientSocket);
	WSACleanup();

	return 0;
}

void error_handling(char* msg) {
	// 프로젝트 -> 속성(맨밑창) -> 문자 집합 -> '멀티바이트 문자 집합 사용'
	MessageBox(NULL, msg, "Error", MB_OK);
	exit(1);
}
