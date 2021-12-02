#include <iostream>
#include <Windows.h>

bool initialize();
FILE* getBinary();
FILE* getBinarybyManual();
HWND hwnd;
errno_t lastError;

UINT8 assembly[] = { 0xC7,0x43,0x34,0x03,0x00,0x00,0x00,0x49,0x8B,0xCF,0xFF,0x15,0xFE,0xA1,0x9A,0x02 };
UINT8 patch[] = { 0xC7,0x43,0x34,0x02,0x00,0x00,0x00,0x49,0x8B,0xCF,0xFF,0x15,0xFE,0xA1,0x9A,0x02 };
int position = 0;

int main()
{
	bool patchSuccess = false;
	UINT8 data;
	if (!initialize())
		return -1;
	FILE* fp = getBinary();
	if (fp == NULL)
		return lastError;
	puts("[+] File open success!");
	while (1)
	{
		size_t count = fread(&data, 1, sizeof(data), fp);
		if (data == assembly[position])
		{
			position++;
		}
		else
			position = 0;
		if (position >= 16)
		{
			printf("[+] Find offset to patch!");
			fseek(fp, -0x10, SEEK_CUR);
			fwrite(patch, 1, sizeof(patch), fp);
			patchSuccess = true;
			break;
		}
		if (count != sizeof(data))
			break;
	}
	if(patchSuccess)
		MessageBox(hwnd, L"Patch success!", L"Success", MB_OK);
	else
		MessageBox(hwnd, L"Patch fail!", L"Faiil", MB_OK | MB_ICONQUESTION);
	fclose(fp);
}

bool initialize()
{
	WCHAR windowTitle[128] = L"4k video downloader patch";
	SetConsoleTitle(windowTitle);
	Sleep(50);
	hwnd = FindWindow(NULL, windowTitle);
	return hwnd != NULL;

}

FILE* getBinary()
{
	FILE* fp;
	errno_t result = fopen_s(&fp, "C:\\Program Files\\4KDownload\\4kvideodownloader\\4kvideodownloader.exe", "rb+");
	lastError = result;
	switch (result)
	{
	case 0:
		return fp;
	case 2:
		MessageBox(hwnd, L"Can't find 4k video downloader. Please select program path.", L"Can't find binary", MB_OK | MB_ICONEXCLAMATION);
		return getBinarybyManual();
	case 13:
		MessageBox(hwnd, L"Permission denied. Please re-open program as admin.\n(Check 4kvideodownloader is already running)", L"Permission Denied", MB_OK | MB_ICONSTOP);
		return NULL;
	default:
		MessageBox(hwnd, L"Error while open 4k video downloader binary. Check exit code", L"Can't find binary", MB_OK | MB_ICONSTOP);
		return NULL;
	}
}

FILE* getBinarybyManual()
{
	OPENFILENAME OFN;
	TCHAR filePathName[100] = L"";
	TCHAR lpstrFile[100] = L"";
	static TCHAR filter[] = L"4k video downloader\0*.exe*\0all files\0*.*";
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hwnd;
	OFN.lpstrFilter = filter;
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = 100;
	OFN.lpstrInitialDir = L".";
	if (GetOpenFileName(&OFN) != 0)
	{
		FILE* fp;
		errno_t result = _wfopen_s(&fp, OFN.lpstrFile, L"rb+");
		lastError = result;
		switch (result)
		{
		case 0:
			return fp;
		case 2:
			MessageBox(hwnd, L"Can't find 4k video downloader. Please select program path.", L"Can't find binary", MB_OK | MB_ICONEXCLAMATION);
			return getBinarybyManual();
		case 13:
			MessageBox(hwnd, L"Permission denied. Please re-open program as admin.\n(Check 4kvideodownloader is already running)", L"Permission Denied", MB_OK | MB_ICONSTOP);
			return NULL;
		default:
			MessageBox(hwnd, L"Error while open 4k video downloader binary. Check exit code", L"Can't find binary", MB_OK | MB_ICONSTOP);
			return NULL;
		}
	}
	else
	{
		MessageBox(hwnd, L"Undefined error!", L"ERROR", MB_OK | MB_ICONSTOP);
		exit(-1);
	}
}