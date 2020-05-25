
#include "lib.h"

void Decrypt_OFB(std::string buffer, std::string& cipher, uint32_t key, uint32_t iv)
{
	uint32_t cur_pos = 0;
	bool flag = true;

	/*
	produce synch
	
	*/

	while (flag)
	{
		uint32_t sync = DecryptBlock(iv, key); // G[i] = F(G[i-1])
		uint32_t x, y_t;

		flag = DecryptTakeBlock(buffer, y_t, cur_pos);

	/*
	xor with synch
	*/
		x = y_t ^ sync;
		iv = sync;


		std::string block;

		Uint32ToString(x, block);

		//printf("Siz_be=%d, Len_C=%d\n", buffer.length(), cipher.length());
		if (flag == false)
		{
			
			uint32_t var1 = cipher.length();
			uint32_t var2 = UnPadding(block);
			//printf("%d-%d=%d", var1, var2, var1 - var2);
			if (var1 == 0)
			{
			
				cipher += block;
			}
			else

			{


				cipher.resize(var1 - var2);

			}
		}
		else
		cipher += block;
	}

	return;
}

void Encrypt_OFB(std::string cipher, std::string& buffer, uint32_t key, uint32_t iv)
{

	/*
produce synch
	
*/   uint32_t cur_pos = 0;
	bool flag = true;
	uint32_t padding_value = 0;
	while ((cipher.length() + padding_value) % 4)
	{
		padding_value++;

	}
	Padding(cipher, padding_value);

	
	do
	{
		uint32_t sync = EncryptBlock(iv, key); // G[i] = F(G[i-1])
		uint32_t x, y_t;

		flag = EncryptTakeBlock(cipher, x, cur_pos);

		y_t = x ^ sync;
		iv = sync;
		sync = EncryptBlock(iv, key); // G[i] = F(G[i-1])

		std::string block;

		Uint32ToString(y_t, block);


		buffer += block;
	} while (flag);

	return;
}
