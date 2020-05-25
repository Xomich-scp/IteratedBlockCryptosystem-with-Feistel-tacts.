#include "lib.h"

#include <iostream>
#include <Windows.h>
#include <vector>
#include <bitset>
#include <thread>
#include <ctime>
#include <random>

//using namespace std;

#define M 4
#define D 4
#define BLOCK_SIZE 32
#define SUBSTITUTION_COUNT 16
#define THREADS 1

std::vector<uint32_t> cycles;

void cyclicalExperimentHandler(uint32_t	firstBlock)
{
	uint32_t key1 = 0x01020304, key2 = 0xf1f2f3f4;
	uint32_t block = firstBlock;
	for (uint32_t i = 0; i < 0xFFFFFFFF; i++)
	{
		block = EncryptBlock(EncryptBlock(block, key1), key2);

		if (block == firstBlock)
		{
			cycles.push_back(i);
			std::cout << "Len of cycleis " << i << " for block " << std::hex << firstBlock << " with keys " << key1 << " and " << key2 << std::endl << std::endl;
			return;
		}
	}
}

void cyclicalExperiment()
{
	clock_t start = clock();
	
	std::thread threads[THREADS];
	std::mt19937 gen(time(NULL));
	
	std::uniform_int_distribution <unsigned int> dis(0x10000000, 0xffffffff);
	
	__GetCurrentTime();

	for (size_t i = 0; i < THREADS; i++)
	{
		uint32_t firstBlock = dis(gen);
		threads[i] = std::thread(cyclicalExperimentHandler, firstBlock);
	}
	
	for (size_t i = 0; i < THREADS; i++)
	{
		threads[i].join();
	}
	
	double end = double(clock() - start) / CLOCKS_PER_SEC;
	
	if (cycles.size() == 0) 
	{
		std::cout << "Cycles lens don`t found" << std::endl;
	}

	std::cout << "Time: " << end << " sec" << std::endl;

	__GetCurrentTime();
}

int main() 
{
	cyclicalExperiment();
	return 0;
}
