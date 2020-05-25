#include "lib.h"

#include <ctime>

// Program for brute force key from 01 and 02 program
// BruteForce.exe open.txt key.txt encrypted.txt

#define FOUNDED_KEY (char *)"key_founded.txt"

// Convert Little Endian to Big Endian

//((x >> 24) | ((x << 8) & 0x00ff0000) | ((x >> 8) & 0x0000ff00) | (x << 24))

void WriteFoundedKey(uint32_t& key_current)
{
	std::string name_file;

	Uint32ToString(key_current, name_file);

	WriteFile(FOUNDED_KEY, name_file);
}

void h(uint32_t key_current, uint32_t& h_key_current, uint32_t constant)
{
	h_key_current = key_current ^ constant;
}

void g(std::string buffer, std::string& new_g, uint32_t constant)
{
	uint32_t cur_pos = 0;
	uint32_t block;

	do
	{
		std::string block_str(buffer, cur_pos, 4);

		std::string block_xor;

		StringToUint32(block_str, block);
		cur_pos += 4;

		block ^= (constant);

		Uint32ToString(block, block_xor);

		new_g += block_xor;

	} while (cur_pos < buffer.length());
}

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		Error("Need args: open_text_file key_file encrypted_file\n"
			"e.g. \"BruteForce.exe open.txt key.txt encrypted.txt\"");
		return 0;
	}

	printf("All output hex value in format Big Endian\n");

	std::string x;

	if (!ReadFile(argv[1], x))
	{
		Error("Bad open text file");
		return 0;
	}

	std::string key_str;

	if (!ReadFile(argv[2], key_str))
	{
		Error("Bad key file");
		return 0;
	}

	uint32_t key;

	if (!StringToUint32(key_str, key))
	{
		Error("Bad key");
		return 0;
	}

	printf("Origin key - 0x%.8X\n", (key));
	printf("[teta_1] xor [teta_2] = 0x%.4X\n", ((uint16_t)((key) >> 16) ^ (uint16_t)(key)));

	std::string y1;

	if (!ReadFile(argv[3], y1))
	{
		Error("Bad encrypted text file");
		return 0;
	}

	uint32_t g1_constant = 0xFFFFFFFF;
	uint32_t g2_constant = 0xFFFFFFFF;
	uint32_t h_constant = 0xFFFFFFFF;

	std::string g1_x;

	g(x, g1_x, g1_constant);

	g1_x.resize(x.size());

	std::string y2;

	Encrypt_Fist(g1_x, y2, key);

#ifdef _DEBUG
	WriteFile((char*)"..\\Debug\\g1_x.txt", g1_x);
	WriteFile((char*)"..\\Debug\\y2.txt", y2);
#else
	WriteFile((char*)"..\\Release\\g1_x.txt", g1_x);
	WriteFile((char*)"..\\Release\\y2.txt", y2);
#endif

	std::srand(unsigned(std::time(0)));

	uint32_t start_value = 0x0;
	uint32_t key_current = start_value;

	//std::cout << "Start value of key: 0x" << std::hex << start_value << std::dec << std::endl;
	printf("Start value of key: 0x%.8X\n", (start_value));

	std::time_t start_time = std::time(0);

	do
	{
		std::string y_current;

		Encrypt_Fist(x, y_current, key_current);

		if (y1 == y_current)
		{
			//std::cout << "Key founded [teta]: 0x" << std::hex << key_current << std::dec << std::endl;
			printf("Key founded [teta]: 0x%.8X\n", (key_current));
			printf("[teta_1]' xor [teta_2]' = 0x%.4X\n", ((uint16_t)((key_current) >> 16) ^ (uint16_t)(key_current)));

			WriteFoundedKey(key_current);

			break;
		}

		std::string g2_y_current;

		g(y_current, g2_y_current, g2_constant);

		if (y2 == g2_y_current)
		{
			uint32_t h_key_current;

			h(key_current, h_key_current, h_constant);

			key_current = (key_current >> 24) | ((key_current << 8) & 0x00ff0000) | ((key_current >> 8) & 0x0000ff00) | (key_current << 24);

			//std::cout << "Key founded [h(teta)]: 0x" << std::hex << h_key_current << std::dec << std::endl;
			printf("Key founded [teta]: 0x%.8X\n", (key_current));
			printf("[teta_1]' xor [teta_2]' = 0x%.4X\n", ((uint16_t)((key_current) >> 16) ^ (uint16_t)(key_current)));

			WriteFoundedKey(key_current);

			break;
		}
		printf("Key looked 0x%.8X\n", (key_current));
		key_current = (key_current + 1);

	} while (key_current != start_value);

	std::cout << std::time(0) - start_time << " seconds" << std::endl;

	return 0;
}
