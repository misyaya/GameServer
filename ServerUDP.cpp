#include "ServerUDP.h"
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment( lib, "ws2_32.lib" )

// ポート番号
const unsigned short SERVERPORT = 8888;
// 送受信するメッセージの最大値
const unsigned int MESSAGELENGTH = 1024;


int main()
{
	std::cout << "Chat Server" << std::endl;


	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0)
	{
		std::cout << "Error: InitWinSock ( ErrorCode:" << ret << " )" << std::endl;
		return 1;	// おーわり！
	}
	std::cout << "Success: InitWinSock" << std::endl;


	// UDPソケットの作成
	int sock;
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		std::cout << "Error: socket ( ErrorCode:" << WSAGetLastError() << " )" << std::endl;
		return 1;	// おーわり！
	}
	std::cout << "Success: socket" << std::endl;


	// 固定アドレスにするためにソケットアドレス情報の割り当て
	struct sockaddr_in bindAddr;
	memset(&bindAddr, 0, sizeof(bindAddr));
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port = htons(SERVERPORT);
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sock, (struct sockaddr*)&bindAddr, sizeof(bindAddr)) < 0)
	{
		std::cout << "Error: bind ( ErrorCode:" << WSAGetLastError() << " )" << std::endl;
		return 1;
	}
	std::cout << "Success: bind" << std::endl;


	while (true)
	{
		char buff[MESSAGELENGTH];			// 送受信メッセージの格納領域
		struct sockaddr_in fromAddr;		// 送信元アドレスの格納領域
		int fromlen = sizeof(fromAddr);		// fromAddrのサイズ

		// 受信待ち
		std::cout << "wait..." << std::endl;

		// 受信	\0は送ってこないバージョン
		ret = recvfrom(sock, buff, sizeof(buff) - 1, 0, (struct sockaddr*)&fromAddr, &fromlen);
		if (ret < 0)
		{
			std::cout << "Error: recvfrom ( ErrorCode:" << WSAGetLastError() << " )" << std::endl;
			return 1;
		}
		buff[ret] = '\0';	// 終端記号追加
		std::cout << "Receive message : " << buff << std::endl;

		// 送信用メッセージの入力
		std::cout << "Input message : ";
		std::cin >> buff;

		// 送信！
		ret = sendto(sock, buff, strlen(buff), 0, (struct sockaddr*)&fromAddr, fromlen);
		if (ret != strlen(buff))
		{
			std::cout << "Error: sendto ( ErrorCode:" << WSAGetLastError() << " )" << std::endl;
			return 1;
		}
	}

	return 0;
}
