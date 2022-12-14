#include "ManageConsoleWindow.h"

void ShowConsoleWindow(bool b)
{
	HWND window;
	AllocConsole();
	window = FindWindowA("ConsoleWindowClass", NULL);

	ShowWindow(window, b);
	
	
}