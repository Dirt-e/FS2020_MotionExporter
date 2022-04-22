#pragma once
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <vector>

//#pragma comment (lib,"ws2_32.lib")
using namespace std;

class UDP_Talker
{
public:
	UDP_Talker(int port);
	~UDP_Talker();

	SOCKET mySocket;
	sockaddr_in myAdress;

	void Talk(string s);
	void Talk(double d);
	void Talk(vector<string> vec);
};

