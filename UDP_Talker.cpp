#include "UDP_Talker.h"
using namespace std;

//Ctor & Dtor
UDP_Talker::UDP_Talker(int port)
{
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (result != 0)
	{
		cout << "UDP_Talker cannot start Winsock :-(" << endl;
	}

	//fill the hint structure for the server. This is who we're talking to.
	myAdress.sin_family = AF_INET;
	myAdress.sin_port = htons(port);
	inet_pton(AF_INET, "127.0.0.1", &myAdress.sin_addr);

	//Create socket
	mySocket = socket(AF_INET, SOCK_DGRAM, 0);
}
UDP_Talker::~UDP_Talker()
{
	//Close socket
	closesocket(mySocket);

	//CleanUp Winsock
	WSACleanup();
}

void UDP_Talker::Talk(string s)
{
	//Write to socket
	int sendok = sendto(mySocket, s.c_str(), (int)s.size(), 0, (sockaddr*)&myAdress, sizeof(myAdress));

	if (sendok == SOCKET_ERROR)
	{
		cout << "Damn It! That didn't work!" << WSAGetLastError << endl;
	}
	else
	{
		cout << "Sending: " + s << endl;
	}
}
void UDP_Talker::Talk(double d)
{
	string s = to_string(d);
	Talk(s);
}
void UDP_Talker::Talk(vector<double> p_vec)
{
	string s = "";

	for (double d : p_vec)
	{
		s.append(to_string(d) + ", ");
	}
	Talk(s);
}