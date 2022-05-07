#pragma warning(disable : 4996)

#include "UDP_Talker.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>

#include "SimConnect.h"
#include "AddGravity.h"

using namespace std;

bool quit = false;

HANDLE hSimConnect = NULL;
UDP_Talker udp_talker(31090);

int Counter = 0;
double Time = 0;
double deltaTime = 0;
double prevTime = 0;

enum DATA_DEFINE_ID {
	DEFINITION_MOTIONDATA,
	//...
};
enum DATA_REQUEST_ID {
	REQUEST_MOTIONDATA,
	//...
};

struct SimResponse {
	double IAS;					//	[0]															
	double MACH;				//	[1]															
	double TAS;					//	[2]															
	double GS;					//	[3]												
	double AOA;					//	[4]												
	double VS;					//	[5]												
	double HGT;					//	[6]

	double BANK;				//	[7]
	double HDG;					//	[8]												
	double PITCH;				//	[9]

	double WX;					//	[10]												
	double WY;					//	[11]												
	double WZ;					//	[12]

	double AX;					//	[13]											
	double AY;					//	[14]												
	double AZ;					//	[15]

	double TIME;				//	[16]	
};

void CALLBACK ProcessData(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext) {
	switch (pData->dwID)
	{
	case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
	{
		SIMCONNECT_RECV_SIMOBJECT_DATA* pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;
		SimResponse* pS = (SimResponse*)&pObjData->dwData;
		switch (pObjData->dwRequestID)
		{
		case REQUEST_MOTIONDATA:
			vector<string> v_MotionData;

			v_MotionData.push_back(to_string(pS->IAS));			//	[0]
			v_MotionData.push_back(to_string(pS->MACH));		//	[1]
			v_MotionData.push_back(to_string(pS->TAS));			//	[2]
			v_MotionData.push_back(to_string(pS->GS));			//	[3]
			v_MotionData.push_back(to_string(pS->AOA));			//	[4]
			v_MotionData.push_back(to_string(pS->VS));			//	[5]
			v_MotionData.push_back(to_string(pS->HGT));			//	[6]

			v_MotionData.push_back(to_string(-pS->BANK));		//	[7]
			v_MotionData.push_back(to_string(pS->HDG));			//	[8]
			v_MotionData.push_back(to_string(-pS->PITCH));		//	[9]

			v_MotionData.push_back(to_string(-pS->WX));			//	[10]
			v_MotionData.push_back(to_string(pS->WY));			//	[11]
			v_MotionData.push_back(to_string(-pS->WZ));			//	[12]

			v_MotionData.push_back(to_string(-pS->WX));			//	[13]
			v_MotionData.push_back(to_string(pS->WY));			//	[14]
			v_MotionData.push_back(to_string(-pS->WZ));			//	[15]

			//Add Gravity (Caution: Pitch is inverted!!!)
			double accLonWithGrav =		AddGravityToAccLon	(pS->AX, -pS->PITCH);
			double accVertWithGrav =	AddGravityToAccVert	(pS->AY, -pS->PITCH, pS->BANK);
			double accLatWithGrav =		AddGravityToAccLat	(pS->AZ, -pS->PITCH, pS->BANK);
			v_MotionData.push_back(to_string(accLonWithGrav));	//	[16]
			v_MotionData.push_back(to_string(accVertWithGrav));	//	[17]
			v_MotionData.push_back(to_string(accLatWithGrav));	//	[18]

			//Calculate times:
			double now = std::chrono::system_clock::now().time_since_epoch().count() / 10000000.0;
			deltaTime = now - prevTime;
			prevTime = now;
			v_MotionData.push_back(to_string(now));				//	[19]
			v_MotionData.push_back(to_string(deltaTime));		//	[20]
			v_MotionData.push_back(to_string(Counter++));		//	[21]
			v_MotionData.push_back("FS2020");					//	[22]

			//Now send the data via UDP
			udp_talker.Talk(v_MotionData);

			break;
		}
		break;
	}

	case SIMCONNECT_RECV_ID_QUIT:
	{
		quit = 1;
		break;
	}

	default:
		break;
	}
}

bool initSimEvents()
{
	// DATA
	SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_MOTIONDATA, "AIRSPEED INDICATED", "meter per second");			//[0]
	SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_MOTIONDATA, "AIRSPEED MACH", "MACH");							//[1]
	SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_MOTIONDATA, "AIRSPEED TRUE", "meter per second");				//[2]
	SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_MOTIONDATA, "GROUND VELOCITY", "meter per second");				//[3]
	SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_MOTIONDATA, "INCIDENCE ALPHA", "degree");						//[4]
	SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_MOTIONDATA, "VERTICAL SPEED", "meter per second");				//[5]
	SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_MOTIONDATA, "RADIO HEIGHT", "meter");							//[6]

	SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_MOTIONDATA, "PLANE BANK DEGREES", "degree");						//[7]
	SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_MOTIONDATA, "PLANE HEADING DEGREES TRUE", "degree");				//[8]
	SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_MOTIONDATA, "PLANE PITCH DEGREES", "degree");					//[9]

	SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_MOTIONDATA, "ROTATION VELOCITY BODY Z", "degree per second");	//[10]
	SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_MOTIONDATA, "ROTATION VELOCITY BODY Y", "degree per second");	//[11]
	SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_MOTIONDATA, "ROTATION VELOCITY BODY X", "degree per second");	//[12]

	SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_MOTIONDATA, "ACCELERATION BODY Z", "meter per second squared");	//[13]
	SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_MOTIONDATA, "ACCELERATION BODY Y", "meter per second squared");	//[14]
	SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_MOTIONDATA, "ACCELERATION BODY X", "meter per second squared");	//[15]

	SimConnect_AddToDataDefinition(hSimConnect, DEFINITION_MOTIONDATA, "ZULU TIME", "second");								//[16]	Time


	SimConnect_RequestDataOnSimObject(hSimConnect, REQUEST_MOTIONDATA, DEFINITION_MOTIONDATA, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_VISUAL_FRAME);

	// Process incoming SimConnect Server messages
	while (!quit)
	{
		// Continuously call SimConnect_CallDispatch until quit - ProcessData will handle simulation events
		SimConnect_CallDispatch(hSimConnect, ProcessData, NULL);
		Sleep(1);
	}

	SimConnect_Close(hSimConnect);
	cout << endl << "FS2020 closed :-(" << endl << endl;

	quit = false;
	return true;
}

int main()
{
	cout << "Starting YAME Motion Data Exporter for FS2020" << endl;

	while (true)
	{
		cout << "Waiting for Connection...";

		while (!SUCCEEDED(SimConnect_Open(&hSimConnect, "Client Event Demo", NULL, 0, NULL, 0)))
		{
			cout << '.';
			Sleep(300);
		}

		cout << endl << endl << "Connected to FS2020 :-)" << endl << endl;

		initSimEvents();
	}

	cin.get();
	return 0;
}