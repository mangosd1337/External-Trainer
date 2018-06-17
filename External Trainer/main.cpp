#include "AdrClass.h"


LPCSTR LGameWindow = "AssaultCube";
std::string GameStatus;

//Funktionsdeklarationen
void WriteToMemory(HANDLE hProchandle);
DWORD FindDmaAdress(int Pointerlevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAdress);

bool IsGameAvailiable;
bool UpdateOnNextRun;

//Speichervariablen und Objekte

//Munitionsobjekt
bool AmmoStatus;
DWORD AmmoOffsets[] = { 0x374, 0x14, 0x0 };
AdrClass* Munition = new AdrClass("\x39\x5", 0x00509B74, 3, AmmoOffsets);

//Lebensobjekt
bool HealthStatus;
DWORD HealthOffsets[] = { 0xF8 };
AdrClass* Leben = new AdrClass("\x39\x5", 0x00509B74, 1, HealthOffsets);

//Granatenobjekt
bool GrenadeStatus;
DWORD GrenadeOffsets[] = { 0x384, 0x14, 0x0 };
AdrClass* Granaten = new AdrClass("\x39\x5", 0x00509B74, 3, GrenadeOffsets);

//Speedobjekt
bool SpeedStatus;
DWORD SpeedOffsets[] = { 0x80 };
AdrClass* Geschwindigkeit = new AdrClass("\x03", 0x00509B74, 1, SpeedOffsets);
AdrClass* DefaultGeschwindigkeit = new AdrClass("\x01", 0x00509B74, 1, SpeedOffsets);



int main()
{
	//Main Funktion Variablen
	HWND hGamewindow = NULL;
	int timeSinceLastUpdate = clock();
	int GameAvailiableTimer = clock();
	int OnePressTimer = clock();
	DWORD dwProcID = NULL;
	HANDLE hProcHandle = NULL;
	UpdateOnNextRun = true;
	std::string sAmmoStatus = "OFF";
	std::string sHealthStatus = "OFF";
	std::string sGrenadeStatus = "OFF";
	std::string sSpeedStatus = "OFF";
	
	while (!GetAsyncKeyState(VK_F12))
	{
		if (clock() - GameAvailiableTimer > 100)
		{
			GameAvailiableTimer = clock();
			IsGameAvailiable = false;
			hGamewindow = FindWindow(NULL, LGameWindow);
			if (hGamewindow)
			{
				GetWindowThreadProcessId(hGamewindow, &dwProcID); //&-Zeichen weil er den Wert in die Variable eintragen soll#
				if (dwProcID != 0)
				{
					hProcHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcID);
					if (hProcHandle == INVALID_HANDLE_VALUE || hProcHandle == NULL)
					{
						GameStatus = "Could not open process!";
					}
					else
					{
						GameStatus = "Ready";
						IsGameAvailiable = true;
					}

				}
				else
				{
					GameStatus = "Process-ID unobtainable";
				}
			}
			else
			{
				GameStatus = "Could not find game";
			}

			if (UpdateOnNextRun || clock() - timeSinceLastUpdate > 5000)
			{
				system("cls");
				std::cout << "------------------------------------------------" << std::endl;
				std::cout << "|                                              |" << std::endl;
				std::cout << "|               AssaultCube External           |" << std::endl;
				std::cout << "|                                              |" << std::endl;
				std::cout << "|                    by mangosd                |" << std::endl;
				std::cout << "------------------------------------------------" << std::endl;
				std::cout << "Gamestatus: " << GameStatus << std::endl << std::endl;
				std::cout << "[F2] Ammo: " << sAmmoStatus << std::endl << std::endl;
				std::cout << "[F3] Health: " << sHealthStatus << std::endl << std::endl;
				std::cout << "[F4] Grenades: " << sGrenadeStatus << std::endl << std::endl;
				std::cout << "[F5] Speed: " << sSpeedStatus << std::endl << std::endl;
				std::cout << "[F12] Disable hack. " << std::endl;
				UpdateOnNextRun = false;
				timeSinceLastUpdate = clock();
			}

			if (IsGameAvailiable)
			{
				WriteToMemory(hProcHandle);
			}
		}

		if (clock() - OnePressTimer > 400)
		{
			if (IsGameAvailiable)
			{
				if (GetAsyncKeyState(VK_F2))
				{
					OnePressTimer = clock();
					AmmoStatus = !AmmoStatus;
					UpdateOnNextRun = true;
					if (AmmoStatus)sAmmoStatus = "ON";
					else sAmmoStatus = "OFF";
				}
				else if (GetAsyncKeyState(VK_F3))
				{
					OnePressTimer = clock();
					HealthStatus = !HealthStatus;
					UpdateOnNextRun = true;
					if (HealthStatus)sHealthStatus = "ON";
					else sHealthStatus = "OFF";
				}
				else if (GetAsyncKeyState(VK_F4))
				{
					OnePressTimer = clock();
					GrenadeStatus = !GrenadeStatus;
					UpdateOnNextRun = true;
					if (GrenadeStatus)sGrenadeStatus = "ON";
					else sGrenadeStatus = "OFF";
				}
				else if (GetAsyncKeyState(VK_F5))
				{
					OnePressTimer = clock();
					SpeedStatus = !SpeedStatus;
					UpdateOnNextRun = true;
					if (SpeedStatus)sSpeedStatus = "ON";
					else
					{
						//Speed ist ein Hurensohn
						sSpeedStatus = "OFF";
						DWORD SpeedAdressToWrite = FindDmaAdress(1, hProcHandle, DefaultGeschwindigkeit->GetClassOffsets(), DefaultGeschwindigkeit->GetClassBaseAdress());
						WriteProcessMemory(hProcHandle, (BYTE*)SpeedAdressToWrite, DefaultGeschwindigkeit->GetClassValue(), sizeof(DefaultGeschwindigkeit->GetClassValue()) - 1, NULL);
					}
				}
			}
		}
	}

	//CloseHandle(hProcHandle); //Willst du das der Scheiﬂ crasht?
	//CloseHandle(hGamewindow); //dann enable den Shit hier.

	return ERROR_SUCCESS;

}



DWORD FindDmaAdress(int Pointerlevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAdress)
{
	DWORD pointer = BaseAdress;
	DWORD pTemp;
	DWORD pointerAdress = 0;
	for (int i = 0; i < Pointerlevel; i++)
	{
		if (i == 0)
		{
			ReadProcessMemory(hProcHandle, (LPCVOID)pointer, &pTemp, 4, NULL);
		}
		
		pointerAdress = pTemp + Offsets[i];

		ReadProcessMemory(hProcHandle, (LPCVOID)pointerAdress, &pTemp, 4, NULL);
	}
	return pointerAdress;
}





void WriteToMemory(HANDLE hProchandle)
{
	if (AmmoStatus)
	{
		DWORD AmmoAdressToWrite = FindDmaAdress(3, hProchandle, Munition->GetClassOffsets(), Munition->GetClassBaseAdress());
		WriteProcessMemory(hProchandle, (BYTE*)AmmoAdressToWrite, Munition->GetClassValue(), sizeof(Munition->GetClassValue())-1 , NULL);
	}

	if (HealthStatus)
	{
		DWORD HealthAdressToWrite = FindDmaAdress(1, hProchandle, Leben->GetClassOffsets(), Leben->GetClassBaseAdress());
		WriteProcessMemory(hProchandle, (BYTE*)HealthAdressToWrite, Leben->GetClassValue(), sizeof(Leben->GetClassValue())-1, NULL);
	}

	if (GrenadeStatus)
	{
		DWORD GrenadeAdressToWrite = FindDmaAdress(3, hProchandle,Granaten->GetClassOffsets(), Granaten->GetClassBaseAdress());
		WriteProcessMemory(hProchandle, (BYTE*)GrenadeAdressToWrite, Granaten->GetClassValue(), sizeof(Granaten->GetClassValue())-1, NULL);
	}

	if (SpeedStatus)
	{
		DWORD SpeedAdressToWrite = FindDmaAdress(1, hProchandle, Geschwindigkeit->GetClassOffsets(), Geschwindigkeit->GetClassBaseAdress());
		WriteProcessMemory(hProchandle, (BYTE*)SpeedAdressToWrite, Geschwindigkeit->GetClassValue(), sizeof(Geschwindigkeit->GetClassValue())-1, NULL);
	}


}
