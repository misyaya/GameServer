#include "ServerUDP.h"
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment( lib, "ws2_32.lib" )

// ポート番号
const unsigned short SERVERPORT = 8888;
// 送受信するメッセージの最大値
const unsigned int MESSAGELENGTH = 1024;


using std::cout;
using std::cin;
using std::endl;
using std::string;

string SetWord();
void Judgement(string _buff);


int main()
{
	cout << "Chat Server" << endl;


	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0)
	{
		cout << "Error: InitWinSock ( ErrorCode:" << ret << " )" << endl;
		return 1;	// 終了
	}
	cout << "Success: InitWinSock" << endl;


	// UDPソケットの作成
	int sock;
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		cout << "Error: socket ( ErrorCode:" << WSAGetLastError() << " )" << endl;
		return 1;	// 終了
	}
	cout << "Success: socket" << endl;


	// 固定アドレスにするためにソケットアドレス情報の割り当て
	struct sockaddr_in bindAddr;
	memset(&bindAddr, 0, sizeof(bindAddr));
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port = htons(SERVERPORT);
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sock, (struct sockaddr*)&bindAddr, sizeof(bindAddr)) < 0)
	{
		cout << "Error: bind ( ErrorCode:" << WSAGetLastError() << " )" << endl;
		return 1; // 終了
	}
	cout << "Success: bind" << endl;


	while (true)
	{
		char buff[MESSAGELENGTH];			// 送受信メッセージの格納領域
		struct sockaddr_in fromAddr;		// 送信元アドレスの格納領域
		int fromlen = sizeof(fromAddr);		// fromAddrのサイズ

		// 受信待ち
		cout << "wait..." << endl;

		// 受信	\0は送ってこないバージョン
		ret = recvfrom(sock, buff, sizeof(buff) - 1, 0, (struct sockaddr*)&fromAddr, &fromlen);
		if (ret < 0)
		{
			cout << "Error: recvfrom ( ErrorCode:" << WSAGetLastError() << " )" << endl;
			return 1;
		}
		buff[ret] = '\0';	// 終端記号追加
		cout << "Receive message : " << buff << endl;
		

		// 送信用メッセージの入力
		cout << "Input message : ";
		string mon = SetWord();
		cin >> buff;
		//buff = SetWord();

		// 送信！
		ret = sendto(sock, buff, strlen(buff), 0, (struct sockaddr*)&fromAddr, fromlen);
		if (ret != strlen(buff))
		{
			cout << "Error: sendto ( ErrorCode:" << WSAGetLastError() << " )" << endl;
			return 1;
		}
	}

	return 0;
}

string SetWord()
{
	int number = 5;

	// 乱数のシードを設定
	srand(static_cast<unsigned>(time(nullptr)));
	string word[5]{ "mmm","nnn","sss","ttt", "lll" };

	int x = rand() % number;

	return word[x];
}

void Judgement(string _buff)
{
	string answer =  SetWord();

	do
	{

	} while (_buff == answer);

	cout << "正解！" << endl;
}
