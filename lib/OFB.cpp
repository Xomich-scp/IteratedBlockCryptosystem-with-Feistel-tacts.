#include "lib.h"

void Decrypt_CFB(std::string buffer, std::string& cipher, uint32_t key, uint32_t iv)
{
	uint32_t cur_pos = 0;
	bool flag = true;

	while (flag)
	{
		uint32_t x, y_t;

		flag = DecryptTakeBlock(buffer, y_t, cur_pos);

		x = y_t ^ y_t_1;
		y_t_1 = EncryptBlock(y_t, key);

		

		std::string block;

		Uint32ToString(x, block);


		if (flag == false)
		{
			UnPadding(block);
		}

		cipher += block;
	}

	return;
}

void Encrypt_CFB(std::string cipher, std::string& buffer, uint32_t key, uint32_t iv)
{
	uint32_t cur_pos = 0;
	bool flag = true;

	

	do
	{
		uint32_t x, y_t;

		flag = EncryptTakeBlock(cipher, x, cur_pos);

		y_t = x ^ y_t_1;
		y_t_1 = EncryptBlock(y_t, key);

		std::string block;

		Uint32ToString(y_t, block);


		buffer += block;
	} while (flag);

	return;
}
