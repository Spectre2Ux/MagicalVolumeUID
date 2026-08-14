#define DISKATTACH "DiskChange"
#define GETUID "GetUid"
#define GETPROGRAMCOMMAND "GetCommand"
#define GETSELECTEDDRIVE "GetSelected"
#define SETDRIVEDEFAULTLETTER "SetDriveLetter"
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <winioctl.h>
#include <stdio.h>
#include <string.h>

HANDLE LogFile;
char LogBuffer[256];
static char SelectedDrive[16] = "\\\\.\\c:";

BOOL MagicVolumeMenu(void);
BOOL GetProgramCommand(void);
BOOL WriteLog(const char* LogText);
BOOL EnableLogFile();
void DiskChangeFunction(void);
void GetMagicUidFunction(void);
void StopWriteToProgramm(void);
void GetSelectedDrive();


int main() {
	memset(LogBuffer, 0, sizeof(LogBuffer));
	EnableLogFile();
	MagicVolumeMenu();
	return 0;
}


BOOL MagicVolumeMenu() {
	char MagicMenuChoice[256];
	printf("MagicVolumeUid created by: Spectre2Ux, Version: Console\n"
		"Program Command - 'GetCommand'\n\n");
	while (1) {
		printf("Command > ");
		scanf("%s", MagicMenuChoice);

		if (strcmp(MagicMenuChoice, DISKATTACH) == 0) {
			WriteLog("User Choice: DISKATTACH");
			DiskChangeFunction();


		}
		else if (strcmp(MagicMenuChoice, GETUID) == 0) {
			WriteLog("User Choice: GETUID");
			GetMagicUidFunction();

		}
		else if (strcmp(MagicMenuChoice, GETPROGRAMCOMMAND) == 0) {
			WriteLog("User Choice: GETPROGRAMCOMMAND");
			GetProgramCommand();

		}
		else if (strcmp(MagicMenuChoice, GETSELECTEDDRIVE) == 0) {
			WriteLog("User Choice: GETSELECTEDDRIVE");
			GetSelectedDrive();
			
		}
		else {
			printf("MagicVolumeUid Error!, Unknown Command\n\n");

		}

	}

	return TRUE;

}


// ------------------
// Functions


void DiskChangeFunction() {
	DWORD AllDrives = GetLogicalDrives();
	int DriveIndex = 0;
	char DiskChoice[256];
	char DriveLetter;
	for (DriveLetter = 'A'; DriveLetter <= 'Z'; DriveLetter++) {
		if (AllDrives & (1 << (DriveLetter - 'A'))) {
			DriveIndex++;
			sprintf(LogBuffer, "Drive Letter Found: %c:\\\n", DriveLetter);
			WriteLog(LogBuffer);
			printf("%d - %c:\\\n", DriveIndex, DriveLetter);
			printf("Select This Disk? Yes/No > ");
			scanf("%s", DiskChoice);
			if (strcmp(DiskChoice, "Yes") == 0) {
				sprintf(SelectedDrive, "\\\\.\\%c:", DriveLetter);
				printf("Selected Drive: \\\\%s\n\n", SelectedDrive);
				WriteLog("Drive Letter Successfully Saved.");
				return;
			}
			else if (strcmp(DiskChoice, "No") == 0) {

			}
		}
	}
}

void GetMagicUidFunction() {
	char GetCatUid[4];
	strcpy(GetCatUid, SelectedDrive + 4);
	strcat(GetCatUid, "\\");
	char DiskType[256];
	DWORD MagicalUid;

	if (GetVolumeInformation(GetCatUid, NULL, 0, &MagicalUid, NULL, NULL, DiskType, sizeof(DiskType))) {
		printf("DiskType: %s\n", DiskType);
		printf("Magical Uid: %08X\n", MagicalUid);
		sprintf(LogBuffer, "DiskType: %s", DiskType);
		WriteLog(LogBuffer);
		sprintf(LogBuffer, "MagicalUid: %08X", MagicalUid);
		WriteLog(LogBuffer);
		return;
	}
	else {
		printf("Error! Not Found Volume Info, Please select disk on 'DiskChange\n\n");
		WriteLog("Error! Not Found Volume Info, Please select disk on 'DiskChange");
		return;
	}
    
	return;
}

BOOL GetProgramCommand() {
	printf("\nProgram Command:\n\n"
		"DiskChange - Change Your Windows Disk.\n"
		"GetUid - Get Magic Uid On Your Selected Disk.\n"
		"GetSelected - Get Selected Drive Letter.\n\n");

	return TRUE;
}

BOOL EnableLogFile() {
	LogFile = CreateFileA("MagicalVolumeUid_Log.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, NULL, NULL);
	if (LogFile == INVALID_HANDLE_VALUE) {
		MessageBoxA(NULL, "LogFile Not Created, Please start this program on Administrator access.\n", "Error, Object: LogFile", MB_ICONERROR);
		return FALSE;
	}
	else {
		SetFilePointer(LogFile, 0, NULL, FILE_END);
		WriteLog("Successfully Open LogFile!");
		return TRUE;
	}

	return TRUE;

}

BOOL WriteLog(const char* LogText) {

	if (LogFile != INVALID_HANDLE_VALUE) {
		char TimeBuffer[256];
		SYSTEMTIME LogSystemTime;
		GetLocalTime(&LogSystemTime);
		sprintf(TimeBuffer, "[ %d:%d:%d ] - %s\n", LogSystemTime.wHour, LogSystemTime.wMinute, LogSystemTime.wSecond, LogText);
		WriteFile(LogFile, &TimeBuffer, (DWORD)strlen(TimeBuffer), NULL, NULL);
		return TRUE;
	}

	else {
		if (LogFile == INVALID_HANDLE_VALUE) {
			MessageBoxA(NULL, "LogFile Not Created, Please start this program on Administrator access.\n", "Error, Object: LogFile", MB_ICONERROR);
		}
		return FALSE;
	}

	return TRUE;

}

void GetSelectedDrive() {
	printf("\nSelected Drive: %s\n\n", SelectedDrive);
	sprintf(LogBuffer, "Selected Drive: %s", SelectedDrive);
	WriteLog(LogBuffer);
	return;
}