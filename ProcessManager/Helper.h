//Helper：一个格式转换的帮助类

#pragma once
class Helper
{
public:
	Helper();
	~Helper();

	//功能：wchar_t* 转 char*
	char* WcharToChar(wchar_t*);

	//功能：char* 转 wchar_t*
	wchar_t* CharToWchar(char*);  

	//功能：FILETIME 转 wchat_t*
	//输出格式样例：1960-01-01 12:00:00
	wchar_t* FileTimeToWChar(FILETIME fileTime);

	//功能：SYSTEMTIME 转 wchat_t*
	//输出格式样例：1960-01-01 12:00:00
	wchar_t* SystemTimeToWChar(SYSTEMTIME systemTime);

	//功能：通过 FILETIME 计算出运行时间并转 wchat_t*
	//输出格式样例：01-12:00:00
	wchar_t* FileTimeAddToRunningTimeToWChar(FILETIME fileTime, FILETIME runningTime);

	//功能：为runningTime加上本次的运行时间
	void FileTimeAddRunningTime(FILETIME *runningTime, FILETIME fileTime);

	//功能：double 转 wchar_t* ，并在结尾加上'M'字符
	//说明：此函数用于打印占用空间大小
	wchar_t* DoubleToWchar_M(double);

private:
	char* m_char;
	wchar_t* m_wchar;

	//释放两个字符串的空间
	void Release();
};
