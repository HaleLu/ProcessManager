#pragma once
class Helper
{
public:
	Helper();
	~Helper();
	char* WcharToChar(wchar_t*);
	wchar_t* CharToWchar(char*); //单字节转宽字节    
	wchar_t* FileTimeToWChar(FILETIME fileTime);
	wchar_t* SystemTimeToWChar(SYSTEMTIME systemTime);
	wchar_t* FileTimeToRunningTimeToWChar(FILETIME fileTime);
	wchar_t* DoubleToWchar_M(double);

private:
	char* m_char;
	wchar_t* m_wchar;

	void Release();
};
