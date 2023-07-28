#include "OverlayCord.h"
#include <iostream>

void Error()
{
	getchar();
	exit(-1);
}

int main()
{
	printf("[>] Searching for target window...\n");
	HWND targetWindow = FindWindowA(nullptr, "Kerbal Space Program");
	if (!targetWindow)
	{
		printf("[!] Target window not found\n");
		Error();
	}

	printf("[+] Target window found with HWND 0x%p\n", targetWindow);

	printf("[>] Resolving process ID...\n");
	UINT targetProcessId;
	GetWindowThreadProcessId(targetWindow, reinterpret_cast<LPDWORD>(&targetProcessId));
	if (!targetProcessId)
	{
		printf("[!] Failed to resolve process ID\n");
		Error();
	}

	printf("[+] Process ID resolved to %u\n", targetProcessId);

	printf("[>] Connecting to the process...\n");
	OverlayCord::Communication::ConnectedProcessInfo processInfo = { 0 };
	processInfo.ProcessId = targetProcessId;
	bool status = OverlayCord::Communication::ConnectToProcess(processInfo);
	if (!status)
	{
		printf("[!] Failed to connect to the process overlay backend\n");
		Error();
	}

	printf("[+] Connected to the process with mapped address 0x%p\n", processInfo.MappedAddress);

	printf("[>] Drawing...\n");
	OverlayCord::Drawing::Frame mainFrame = OverlayCord::Drawing::CreateFrame(1280, 720);

	const UINT rectangleWidth = 100;
	const UINT rectangleHeight = 200;
	constexpr double speed = 200.0;

	double currentPosition = 0.0;
	int direction = 1;
	OverlayCord::Drawing::Pixel color = { 0, 0, 255, 255 };

	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	LARGE_INTEGER t1, t2;
	QueryPerformanceCounter(&t1);

	while (!(GetKeyState(VK_INSERT) & 0x8000))
	{
		QueryPerformanceCounter(&t2);

		double deltaTime = (t2.QuadPart - t1.QuadPart) / static_cast<double>(frequency.QuadPart);
		t1 = t2;

		OverlayCord::Drawing::CleanFrame(mainFrame);

		OverlayCord::Drawing::DrawRectangle(mainFrame, static_cast<UINT>(currentPosition), mainFrame.Height / 2 - rectangleHeight / 2, rectangleWidth, rectangleHeight, color);

		currentPosition += direction * speed * deltaTime;

		if (currentPosition + rectangleWidth >= mainFrame.Width)
			direction = -1;
		else if (currentPosition <= 0)
			direction = 1;

		OverlayCord::Communication::SendFrame(processInfo, mainFrame.Width, mainFrame.Height, mainFrame.Buffer, mainFrame.Size);
	}

	getchar();
	return 0;
}