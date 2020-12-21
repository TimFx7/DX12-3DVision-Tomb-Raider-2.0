// Activates 3D by manipulating memory for Shadow of the Tomb Raider
//

#include "Start3D.h"
#include <windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <vector>


int main()
{
	HWND GameWindow;
	int kez = 0;

	GameWindow = ::FindWindowExA(0, 0, "TR11NxApp", 0); // Tomb Raider Window Class Name 
	while ((GameWindow == NULL) & (kez <= 60))
	{
		Sleep(1000);
		GameWindow = ::FindWindowExA(0, 0, "TR11NxApp", 0);
		kez++;
		if (kez == 60)
		{
			MessageBox(nullptr, TEXT("Shadow of the Tomb Raider uninitialized. \nPlease start the game first !!!"), TEXT("WARNING : Enabling 3D Vision"), MB_SYSTEMMODAL | MB_ICONEXCLAMATION | MB_OK);
			return 0;
		}
	}

	//Get ProcId of the target process
	DWORD procId = GetProcId(L"SOTTR.exe");  //Tomb Raider 

	//Getmodulebaseaddress
	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"SOTTR.exe");

	//Get Handle to Process
	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	//Ingame options, resolve base address of the pointer chain    
	uintptr_t dynamicPtrBaseAddr = moduleBase + 0x36905B0;

	//...Game menu >> Options >> Display and Graphics >> Display >> Resolve Stereoscopic pointer chain
	std::vector<unsigned int> StereoscopicOffsets = { 0x90, 0x68, 0x68, 0x30, 0x38, 0xB0, 0x924 }; // memory address of SOTTR 3D Vision key
	uintptr_t StereoscopicAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, StereoscopicOffsets);

	//Write to it
	int Stereoscopic = 2;  // 3D Vision ON  (0= off  1= SBS  2= 3D Vision)

	// Each time the window of the game is seen, it enters into memory the value of 3D Vision.
	while (FindWindowExA(0, 0, "TR11NxApp", 0)) {
		WriteProcessMemory(hProcess, (BYTE*)StereoscopicAddr, &Stereoscopic, sizeof(Stereoscopic), nullptr);
		Sleep(1000);
	}


	getchar();
	return 0;
}