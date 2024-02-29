#include "ServerUDP.h"
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <cstring>
#include <algorithm>

#pragma comment( lib, "ws2_32.lib" )

// ポート番号
const unsigned short SERVERPORT = 8888;
// 送受信するメッセージの最大値
const unsigned int MESSAGELENGTH = 1024;


using std::cout;
using std::cin;
using std::endl;
using std::string;

string answer;
string standby;

int num = 0;

int Recv(int sock, char* buff, sockaddr_in &fromAddr, int fromlen);
int Send(int sock, char* buff, sockaddr_in& fromAddr, int fromlen);
int reSend(int sock, char* buff, sockaddr_in& fromAddr, int fromlen);

string SetWord();
bool Judgement(string _buff);


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

	char buff[MESSAGELENGTH];			// 送受信メッセージの格納領域
	//char buff[MESSAGELENGTH];			// 送受信メッセージの格納領域
	struct sockaddr_in fromAddr;		// 送信元アドレスの格納領域
	int fromlen = sizeof(fromAddr);		// fromAddrのサイズ

	// 受信待ち
	cout << "wait..." << endl;

	Recv(sock, buff, fromAddr, fromlen);
	standby = buff;

	answer = SetWord();
	// 文字列をchar配列にコピー
	size_t length = min(answer.length(), static_cast<size_t>(MESSAGELENGTH - 1));
	strncpy_s(buff, MESSAGELENGTH, answer.c_str(), length);
	buff[length] = '\0';  // 終端文字を追加

	Send(sock, buff, fromAddr, fromlen);

	while (true)
	{
		char buff[MESSAGELENGTH];			// 送受信メッセージの格納領域
		
		// 受信待ち
		cout << "wait..." << endl;
		Recv(sock, buff, fromAddr, fromlen);

			while (!Judgement(standby))
			{
				string one = "残念！　もう一度";

				// 文字列をchar配列にコピー
				size_t length = min(one.length(), static_cast<size_t>(MESSAGELENGTH - 1));
				strncpy_s(buff, MESSAGELENGTH, one.c_str(), length);
				buff[length] = '\0';  // 終端文字を追加

				Send(sock, buff, fromAddr, fromlen);

				Recv(sock, buff, fromAddr, fromlen);
			}
		

		answer = SetWord();
		string one = "正解！　おめでとう\n　　　　　　　　　次のお題　" + answer;

		// 文字列をchar配列にコピー
		length = min(one.length(), static_cast<size_t>(MESSAGELENGTH - 1));
		strncpy_s(buff, MESSAGELENGTH, one.c_str(), length);
		buff[length] = '\0';  // 終端文字を追加
		Send(sock, buff, fromAddr, fromlen);
	}

	return 0;
}

int Recv(int sock, char* buff, sockaddr_in &fromAddr, int fromlen)
{
	// 受信
	int ret = recvfrom(sock, buff, MESSAGELENGTH - 1, 0, (struct sockaddr*)&fromAddr, &fromlen);
	if (ret < 0)
	{
		cout << "Error: recvfrom ( ErrorCode:" << WSAGetLastError() << " )" << endl;
		return 1;
	}

	buff[ret] = '\0'; // 受信したデータを終端記号で終える
	cout << "Receive message : " << buff << endl;
	standby = buff;
	return 0;
}

int Send(int _sock, char* _buff, sockaddr_in& _fromAddr, int _fromlen)
{
	// 送信！
	int ret = sendto(_sock, _buff, strlen(_buff), 0, (struct sockaddr*)&_fromAddr, _fromlen);
	if (ret != strlen(_buff))
	{
		cout << "Error: sendto ( ErrorCode:" << WSAGetLastError() << " )" << endl;
		return 1;
	}
	return 0;
}

int reSend(int _sock, char* _buff, sockaddr_in& _fromAddr, int _fromlen)
{
	// 送信！
	int ret = sendto(_sock, _buff, strlen(_buff), 0, (struct sockaddr*)&_fromAddr, _fromlen);
	if (ret != strlen(_buff))
	{
		cout << "Error: sendto ( ErrorCode:" << WSAGetLastError() << " )" << endl;
		return 1;
	}
	return 0;
}

string SetWord()
{
	int number = 10;

	// 乱数のシードを設定
	srand(static_cast<unsigned>(time(nullptr)));
	string word[10]{ "agent",
					 "cyclone",
					 "joker",
					 "stylish", 
					 "sophisticated",
					 "abyss",
					 "eternity",
					 "arcana",
					 "masquerade",
					 "odyssey"};

	int x = rand() % number;

	return word[x];
}

bool Judgement(string _buff)
{
	if (_buff != answer)
	{
		return false;
	}

	return true;
}
