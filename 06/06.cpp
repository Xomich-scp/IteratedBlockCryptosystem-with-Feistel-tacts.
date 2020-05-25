#include "lib.h"
/*
��� ��� ����� ��������� ���� �� ����� 6-7 (��� �� ���� ���� �� 1 �� ���).
��� ���������� ����������� ��������� ���, 
����� ��� ��������� �� ���� ����� 4-�� ��������� �����������. 
������ ������� - ����� ��������� ����, � ������� ����������� ����� ���������, ��������, ���:
{1, 14, 2, 15, 3, 6, 7, 8, 9, 10, 4, 5, 12, 11, 0, 13}
��������� ������ �������� � ��������� ���� 2 �������. 

1 ������� - ��������� ������� ��������� (�������� �������������), 
����������� - �������, � ������ ������ �� 15 �����, ��������������. 
� ����� ������ ������� ���� ������ (��� ��������� ����������). 

2-� ������� �������� 3 �������: ���, ����������� � ������ �������������� (�������������) 
� ������� [a, b] ����� �������. 


�.�. ������ ������ �� ������ ������� ����� ����� ���������� �����.
*/

using namespace std;
int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		Error("Need args: tabin.txt "
			"e.g. \"06.exe tabin.txt \"");
		return 0;
	}
	//system("pause");
	std::string cipher;
	
	if (!ReadFile(argv[1], cipher))
	{
		Error("Bad input file");
		return 0;
	}
	int Sbox[16];
	
	float table[16][16] = { 0.0 };
	for (int i =0; i<16;i++)
	{
		
		int start_point = cipher.find(",");
		string number = cipher.substr(0, start_point);
		
		if (number.find(" ")!=string::npos)
		{
			
			number.erase(0, number.find(" ")+1);
		}

		if (number.find("{") != string::npos)
		{
			number.erase(0, number.find("{")+1);			
		}

		Sbox[i] = atoi(number.c_str());
		cipher.erase(0, start_point+1);
		
	}

	differ_table(Sbox);
	liner_table(Sbox);
	//WriteFile(argv[3], cipher);
	return 0;
}
