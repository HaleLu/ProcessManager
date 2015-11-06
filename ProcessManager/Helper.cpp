#include "stdafx.h"
#include "Helper.h"


Helper::Helper():m_char(NULL),m_wchar(NULL)
{
}


Helper::~Helper()
{
	Release();
}

char* Helper::WcharToChar(wchar_t* wc)
{
	Release();
	int len = WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), NULL, 0, NULL, NULL);
	m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}

wchar_t* Helper::CharToWchar(char* c)
{
	Release();
	int len = MultiByteToWideChar(CP_ACP, 0, c, strlen(c), NULL, 0);
	m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, c, strlen(c), m_wchar, len);
	m_wchar[len] = '\0';
	return m_wchar;
}

wchar_t* Helper::FileTimeToWChar(FILETIME fileTime)
{
	
	FILETIME localTime;
	FileTimeToLocalFileTime(&fileTime, &localTime);
	SYSTEMTIME systemTime;
	FileTimeToSystemTime(&localTime, &systemTime);

	Release();
	m_char = new char[25];
	sprintf(m_char, "%04d-%02d-%02d-%02d:%02d:%02d",
		systemTime.wYear, systemTime.wMonth,
		systemTime.wDay, systemTime.wHour,
		systemTime.wMinute, systemTime.wSecond);
	
	auto len = MultiByteToWideChar(CP_ACP, 0, m_char, strlen(m_char), NULL, 0);
	m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, m_char, strlen(m_char), m_wchar, len);
	m_wchar[len] = '\0';

	return m_wchar;
}

wchar_t* Helper::SystemTimeToWChar(SYSTEMTIME systemTime)
{
	Release();
	m_char = new char[25];
	sprintf(m_char, "%04d-%02d-%02d-%02d:%02d:%02d",
		systemTime.wYear, systemTime.wMonth,
		systemTime.wDay, systemTime.wHour,
		systemTime.wMinute, systemTime.wSecond);

	auto len = MultiByteToWideChar(CP_ACP, 0, m_char, strlen(m_char), NULL, 0);
	m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, m_char, strlen(m_char), m_wchar, len);
	m_wchar[len] = '\0';

	return m_wchar;
}

wchar_t* Helper::FileTimeToRunningTimeToWChar(FILETIME fileTime)
{
	SYSTEMTIME currentSystemTime;
	GetSystemTime(&currentSystemTime);
	FILETIME currentTime;
	SystemTimeToFileTime(&currentSystemTime, &currentTime);

	FILETIME runningTime;

	if (currentTime.dwLowDateTime > fileTime.dwLowDateTime)
	{
		runningTime.dwHighDateTime = currentTime.dwHighDateTime - fileTime.dwHighDateTime;
		runningTime.dwLowDateTime = currentTime.dwLowDateTime - fileTime.dwLowDateTime;
	}
	else
	{
		runningTime.dwHighDateTime = currentTime.dwHighDateTime - fileTime.dwHighDateTime - 1;
		runningTime.dwLowDateTime = currentTime.dwLowDateTime - fileTime.dwLowDateTime;
	}

	SYSTEMTIME systemTime;
	FileTimeToSystemTime(&runningTime, &systemTime);
	
	Release();
	m_char = new char[25];
	sprintf(m_char, "%02d-%02d:%02d:%02d",
		systemTime.wDay - 1, systemTime.wHour,
		systemTime.wMinute, systemTime.wSecond);
	
	auto len = MultiByteToWideChar(CP_ACP, 0, m_char, strlen(m_char), NULL, 0);
	m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, m_char, strlen(m_char), m_wchar, len);
	m_wchar[len] = '\0';

	return m_wchar;
}

wchar_t* Helper::DoubleToWchar_M(double size_x)
{
	Release();
	m_char = new char[25];
	sprintf(m_char,"\t%.1fM", size_x);

	auto len = MultiByteToWideChar(CP_ACP, 0, m_char, strlen(m_char), NULL, 0);
	m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, m_char, strlen(m_char), m_wchar, len);
	m_wchar[len] = '\0';

	return m_wchar;
}

void Helper::Release()
{
	if (m_char)
	{
		delete m_char;
		m_char = NULL;
	}
	if (m_wchar)
	{
		delete m_wchar;
		m_wchar = NULL;
	}
}
