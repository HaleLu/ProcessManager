#pragma once
class Helper
{
public:
	Helper();
	~Helper();
	char* WcharToChar(wchar_t*);
	wchar_t* CharToWchar(char* c); //单字节转宽字节    
	
private:
	char* m_char;
	wchar_t* m_wchar;

	void Release();
};
