#include <windows.h>
#include <stdio.h>

// g (Global optimization), s (Favor small code), y (No frame pointers).
#pragma optimize("gsy", on)
#pragma comment(linker, "/OPT:NOWIN98")		// Make section alignment really small.
#define WIN32_LEAN_AND_MEAN

int main(int argc, char *argv[]){
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	DWORD	dwTimeAdjustments= 400000;
	
	// Get privileges needed for fiddling with the clock.
	// Get the current process token handle.
	if( !OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken) ){
		MessageBoxA(NULL, "OpenProcessToken failed.", "OpenProcessToken Failed.", MB_OK);
		exit(-1);
	}
	// Get the LUID for system-time privilege.
	LookupPrivilegeValue(NULL, SE_SYSTEMTIME_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount= 1;  /* one privilege to set */
	tkp.Privileges[0].Attributes= SE_PRIVILEGE_ENABLED;

	// Get set-time privilege for this process. */
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0);
	
	// Cannot use return value of AdjustTokenPrivileges.
	// (success does not indicate all privileges were set).
	if( GetLastError()!=ERROR_SUCCESS ){
		MessageBoxA(NULL, "AdjustTokenPrivileges failed.", "AdjustTokenPrivileges failed.", MB_OK);
		// later set time call will probably fail.
	}
	
	// Set the Clock to a reasonable increment.
	if( !SetSystemTimeAdjustment(dwTimeAdjustments, FALSE) ){
		MessageBoxA(NULL, "SetSystemTimeAdjustment failed.", "SetSystemTimeAdjustment failed.", MB_OK);
		exit (-1);
	}
	return 0;
}
