#include <Windows.h>
#include <iostream>
#include <string>
#include <bitset>
#include <thread>

INPUT inp;
bool running = true;

void pressKey() {
	while (running)
		SendInput(1, &inp, sizeof(INPUT));
}

int main() {
	HANDLE hStdin;
	DWORD fdwMode;

	std::cout << "Press a key or button to register as the PTT Key" << std::endl;


	int pttKey = -1;

	while (pttKey == -1) {
		for (int i = 0; i < 256; i++) {
			byte b = GetAsyncKeyState(i);
			if (b == 1) {
				pttKey = i;
				break;
			}
		}
	}


	DWORD mouseMapDown[6] = { MOUSEEVENTF_LEFTDOWN,MOUSEEVENTF_RIGHTDOWN,NULL,MOUSEEVENTF_MIDDLEDOWN,MOUSEEVENTF_XDOWN,MOUSEEVENTF_XDOWN };
	DWORD mouseMapUp[6] = { MOUSEEVENTF_LEFTUP,MOUSEEVENTF_RIGHTUP,NULL,MOUSEEVENTF_MIDDLEUP,MOUSEEVENTF_XUP,MOUSEEVENTF_XUP };

	inp = { 0 };

	if (pttKey <= 0x06) {
		inp.type = INPUT_MOUSE;
		inp.mi.dwFlags = mouseMapDown[pttKey - 1];
		if (pttKey == 0x05) {
			inp.mi.mouseData = 0x0001;
		} else if (pttKey == 0x06) {
			inp.mi.mouseData = 0x0002;
		}
	} else {
		inp.type = INPUT_KEYBOARD;
		inp.ki.dwFlags = 0;
		inp.ki.wVk = pttKey;
	}

	std::thread keyThread;


	if (pttKey <= 0x06) {
		inp.mi.dwFlags = mouseMapDown[pttKey - 1];
	} else {
		inp.ki.dwFlags = 0;
	}

	keyThread = std::thread(&pressKey);

	std::cout << "Started! Press enter to exit!" << std::endl;


	std::string enterToExit;
	std::getline(std::cin, enterToExit);

	if (pttKey <= 0x06) {
		inp.mi.dwFlags = mouseMapUp[pttKey - 1];
	} else {
		inp.ki.dwFlags = 2;
	}


	running = false;
	keyThread.join();


	SendInput(1, &inp, sizeof(INPUT));

	return 0;
}