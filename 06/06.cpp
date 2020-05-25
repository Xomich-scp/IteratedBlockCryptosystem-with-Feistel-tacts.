#include "lib.h"
/*
Там еще будет отдельный тест по лабам 6-7 (кто не сдал хотя бы 1 из них).
Вам необходимо подготовить программы так, 
чтобы они принимали на вход любую 4-ёх разрядную подстановку. 
Формат простой - будет текстовый файл, в котором подстановка будет выглядеть, например, так:
{1, 14, 2, 15, 3, 6, 7, 8, 9, 10, 4, 5, 12, 11, 0, 13}
Программа должна выводить в текстовый файл 2 таблицы. 

1 таблица - урезанная таблица разностей (линейных аппроксимаций), 
разделитель - запятая, в каждой строке по 15 чисел, соответственно. 
В конце строки запятую тоже ставим (для упрощения реализации). 

2-я таблица содержит 3 столбца: вес, вероятность и список дифференциалов (аппроксимаций) 
в формате [a, b] через запятую. 


Т.о. каждая строка во второй таблице будет иметь переменную длину.
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
