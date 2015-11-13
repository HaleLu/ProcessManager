//Helper��һ����ʽת���İ�����

#pragma once
class Helper
{
public:
	Helper();
	~Helper();

	//���ܣ�wchar_t* ת char*
	char* WcharToChar(wchar_t*);

	//���ܣ�char* ת wchar_t*
	wchar_t* CharToWchar(char*);  

	//���ܣ�FILETIME ת wchat_t*
	//�����ʽ������1960-01-01 12:00:00
	wchar_t* FileTimeToWChar(FILETIME fileTime);

	//���ܣ�SYSTEMTIME ת wchat_t*
	//�����ʽ������1960-01-01 12:00:00
	wchar_t* SystemTimeToWChar(SYSTEMTIME systemTime);

	//���ܣ�ͨ�� FILETIME ���������ʱ�䲢ת wchat_t*
	//�����ʽ������01-12:00:00
	wchar_t* FileTimeAddToRunningTimeToWChar(FILETIME fileTime, FILETIME runningTime);

	//���ܣ�ΪrunningTime���ϱ��ε�����ʱ��
	void FileTimeAddRunningTime(FILETIME *runningTime, FILETIME fileTime);

	//���ܣ�double ת wchar_t* �����ڽ�β����'M'�ַ�
	//˵�����˺������ڴ�ӡռ�ÿռ��С
	wchar_t* DoubleToWchar_M(double);

private:
	char* m_char;
	wchar_t* m_wchar;

	//�ͷ������ַ����Ŀռ�
	void Release();
};
