#define _CRT_SECURE_NO_WARNINGS // GetCurrentTime

#include "lib.h"

#include <iomanip> // GetCurrentTime
#include <bitset> // DisplayUin32IntoGroupsOf4Bits

/*

Some routine to implement big-endian reading/writing and some debug function

*/
int StringToUint32(std::string block_str, uint32_t & block)
{


    if (block_str.length() > 4)
    {
        return 0;
    }

    block = (*((uint32_t *)block_str.c_str()));

	uint16_t leftPart;// = (uint16_t)(block >> 16);
	uint16_t rightPart;// = (uint16_t)((block << 16) >> 16);
	uint8_t l_left;
	uint8_t l_right;

	leftPart = (uint16_t)(block >> 16);
	l_left = (uint8_t)(leftPart >> 8);
	l_right = (uint8_t)((leftPart << 8)>>8);
	leftPart = ((uint16_t)l_right << 8) | l_left;


	rightPart = (uint16_t)((block << 16) >> 16);
	l_left = (uint8_t)(rightPart >> 8);
	l_right = (uint8_t)((rightPart << 8) >> 8);
	rightPart = ((uint16_t)l_right << 8) | l_left;

	block = ((uint32_t)rightPart << 16) | leftPart;

    return 1;
}

void Uint32ToString(uint32_t & block, std::string & block_str)
{

	uint16_t leftPart;// = (uint16_t)(block >> 16);
	uint16_t rightPart;// = (uint16_t)((block << 16) >> 16);
	uint8_t l_left;
	uint8_t l_right;

	leftPart = (uint16_t)(block >> 16);
	l_left = (uint8_t)(leftPart >> 8);
	l_right = (uint8_t)((leftPart << 8) >> 8);
	leftPart = ((uint16_t)l_right << 8) | l_left;


	rightPart = (uint16_t)((block << 16) >> 16);
	l_left = (uint8_t)(rightPart >> 8);
	l_right = (uint8_t)((rightPart << 8) >> 8);
	rightPart = ((uint16_t)l_right << 8) | l_left;

	block = ((uint32_t)rightPart << 16) | leftPart;

    block_str = std::string((char *)& block, 4);
}

void Error(char const * msg)
{
    std::cout << "Error: " << msg << std::endl;
}

void GetCurrentTime()
{
	auto time = std::time(nullptr);

	// ISO 8601: %Y-%m-%d %H:%M:%S, e.g. 2017-07-31 00:42:00+0200.
	std::cout << "Start time: " << std::put_time(std::gmtime(&time), "%F %T%z") << std::endl;
}

void DisplayMsgAndUin32IntoGroupsOf4Bits(char const * msg, uint32_t number)
{
	std::cout << msg;

	auto bits = std::bitset<32>(number);
	int count = 0;

	for (std::size_t i = 0; i < bits.size(); ++i)
	{
		if (i && i % 4 == 0)
		{
			// write a space before starting a new nibble (except before the very first nibble)
			std::cout << ' ';
		}
		std::cout << bits[bits.size() - i - 1];
	}

	std::cout << std::endl;
}


