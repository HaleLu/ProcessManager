#pragma once
class Helper
{
public:
	Helper();
	~Helper();
	char* WcharToChar(wchar_t*);
	wchar_t* CharToWchar(char* c); //���ֽ�ת���ֽ�    
	
private:
	char* m_char;
	wchar_t* m_wchar;

	void Release();
};
