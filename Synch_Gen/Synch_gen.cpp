#include "lib.h"
#include <ctime>
#include <string>
using namespace std;
int main()

{
	for (int i = 1; i < 11; i++)
	{
		
		
		string path = "IV" + to_string(i) + ".in";
		std::ofstream file(path, std::ios_base::binary);
		file.clear();
		for (int j = 0; j < 4; j++)
		{
			uint8_t sync = rand();
			file << sync;
		}
		
		file.close();
	
	}
}
/*
	std::ofstream file(path, std::ios_base::binary);
		file.clear();
		file << sync;
		file.close();
*/