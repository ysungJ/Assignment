#include <stdio.h>
#include <windows.h>


int main(void) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	TCHAR szCmdLine[] = TEXT("C:\\Windows\\System32\\mspaint.exe");
	
	if (!CreateProcess(NULL, szCmdLine,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi)
		)
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		return -1;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);


	return 0;
}