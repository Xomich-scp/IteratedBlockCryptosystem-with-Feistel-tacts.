#include "lib.h"

#include <vector>
#include <random>
#include <ctime>

// Program for group analysis cryptosystem from lab_02
// > lab_05_GroupAnalysis.exe

std::vector<uint32_t> cycles;

void cyclicalExperimentHandler(size_t iteration, uint32_t	firstBlock, uint32_t key1, uint32_t key2)
{
	uint32_t block = firstBlock;
	
	clock_t start = clock();

	for (uint32_t i = 0; i < 0xFFFFFFFF; i++)
	{
		block = EncryptBlock(EncryptBlock(block, key1), key2);

		if (block == firstBlock)
		{
			cycles.push_back(i);
			printf("%2d    0x%.8X    0x%.8X    0x%.8X    %d\t%f\n", iteration, key1, key2, firstBlock, i + 1, double(clock() - start) / CLOCKS_PER_SEC);
			//std::cout << "Len of cycleis " << i + 1 << " for block " << std::hex << firstBlock << " with keys " << key1 << " and " << key2 << std::endl;
			return;
		}
	}
}

void cyclicalExperiment()
{
	clock_t start = clock();

	std::mt19937 gen((uint32_t)time(NULL));
	std::uniform_int_distribution <uint32_t> dis(0x10000000, 0xffffffff);

	GetCurrentTime();

	//cyclicalExperimentHandler(0x4ae13d6c, 0x2cd672ae, 0x18be6784);
	printf("Try#      key1        key2        block    len\tTime\n");

	for (size_t i = 0; i < 10; i++)
	{
		cyclicalExperimentHandler(i, dis(gen), dis(gen), dis(gen));
	}

	if (cycles.size() == 0)
	{
		std::cout << "Cycles lens don`t found" << std::endl;
	}

	std::cout << "Time: " << double(clock() - start) / CLOCKS_PER_SEC << " sec" << std::endl;

	GetCurrentTime();
}

int main()
{
	cyclicalExperiment();
	return 0;
}
