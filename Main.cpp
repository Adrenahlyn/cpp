#include <iostream>
#include <Windows.h>
#include <ctime>

std::string GameName = "AssaultCube";
LPCSTR LGameWindow = "AssaultCube";
std::string GameStatus;


bool isGameAvail;
bool UpdateOnNextRun;

// Ammo
bool AmmoStatus;
BYTE AmmoValue[] = { 0xA3, 0x1C, 0x0, 0x0 };
DWORD AmmoBaseAddress = { 0x00509B74 };
DWORD AmmoOffsets[] = { 0x384, 0x14, 0x0 };


// Health
bool HealthStatus;
BYTE HealthValue[] = { 0x39, 0x5, 0x0, 0x0 };
DWORD HealthBaseAddress = { 0x00509B74 };
DWORD HealthOffsets[] = { 0xF8 };

int main()
{
	HWND hGameWindow = NULL;
	int timeSinceLastUpdate = clock();
	int GameAvailTimer = clock();
	int onePressTimer = clock();
	DWORD dwProcID = NULL;
	HWND hProcHandle = NULL;
	UpdateOnNextRun = true;
	std::string sAmmoStatus = "OFF";
	std::string sHealthStatus = "OFF";

	while (!GetAsyncKeyState(VK_INSERT))
	{
		if (clock() - GameAvailTimer > 100)
		{
			GameAvailTimer = clock();
			isGameAvail = false;

			hGameWindow = FindWindow(NULL, LGameWindow);
			if (hGameWindow)
			{
				GetWindowThreadProcessId(hGameWindow, &dwProcID);
				if (dwProcID != 0)
				{
					hProcHandle = OpenProcess(PROCESS_ALL_ACCESS, false, dwProcID);
					if (hProcHandle == INVALID_HANDLE_VALUE || hProcHandle == NULL)
					{
						GameStatus = "Failed to open process for valid handle";
					}
					else
					{
						GameStatus = "AssaultCube ready to hack";
						isGameAvail = true;

					}
				}
				else
				{
					GameStatus = "Failed to get process Id";
				}
			}
			else
			{
				GameStatus = "AssaultCube NOT FOUND";
			}
			if (UpdateOnNextRun || clock() - timeSinceLastUpdate > 5000)
			{
				system("cls");
				std::cout << "----------------------------------------------------" << std::endl;
				std::cout << "        AssaultCube memory hacker" << std::endl;
				std::cout << "----------------------------------------------------" << std::endl << std::endl;
				std::cout << "GAME STATUS:"<< GameStatus  <<"   " << std::endl << std::endl;
				std::cout << "[F1] Unlimited ammo -> "<< sAmmoStatus <<" <-" << std::endl<< std::endl;
				std::cout << "[F2] Unlimited Health and armor ->" << sHealthStatus << "<-" << std::endl<< std::endl;
				std::cout << "[INSERT] Exit" << std::endl;
				UpdateOnNextRun = false;
				timeSinceLastUpdate = clock();

			}
			if (isGameAvail)
			{

			}
		}
		if (clock() - onePressTimer > 400)
		{
			if (isGameAvail)
			{
				if (GetAsyncKeyState(VK_F1))
				{
					onePressTimer = clock();
					AmmoStatus = !AmmoStatus;
					UpdateOnNextRun = true;
					if (AmmoStatus)
					{
						sAmmoStatus = "ON";
					}
					else
					{
						sAmmoStatus = "OFF";
					}
				}
				if (GetAsyncKeyState(VK_F2))
				{
					onePressTimer = clock();
					HealthStatus = !HealthStatus;
					UpdateOnNextRun = true;
					if (HealthStatus)
					{
						sHealthStatus = "ON";
					}
					else
					{
						sHealthStatus = "OFF";
					}
				}
			}
		}
	}
	CloseHandle(hProcHandle);
	CloseHandle(hGameWindow);

	return ERROR_SUCCESS;
}

DWORD FindDmaAddy(int PointerLevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAddress)
{
	DWORD pointer = BaseAddress;
	DWORD pTemp;
	DWORD pointerAddr;
	for (int c = 0; c < PointerLevel; c++)
	{
		if (c == 0)
		{
			ReadProcessMemory(hProcHandle, (LPCVOID)pointer, &pTemp, sizeof(pTemp), 4, NULL)
		}

		pointerAddr = pTemp + Offsets[c];
		ReadProcessMemory(hProcHandle, (LPCVOID)pointer, &pTemp, sizeof(pTemp), 4, NULL);
	}
	return pointerAddr;
}

void WriteToMemory(HANDLE hProcHandle)
{
	if (AmmoStatus)
	{
		DWORD AmmoAddressToWrite = FindDmaAddy(3, hProcHandle, AmmoOffsets, AmmoBaseAddress);
		WriteProcessMemory(hProcHandle, (BYTE)AmmoAddressToWrite, &AmmoValue, sizeof(AmmoValue), NULL);
	}
	if (HealthStatus){
		DWORD HealthAddressToWrite = FindDmaAddy(1, hProcHandle, HealthOffsets, HealthBaseAddress);
		WriteProcessMemory(hProcHandle, (BYTE)HealthAddressToWrite, &HealthValue, sizeof(HealthValue), NULL);
	}
}
