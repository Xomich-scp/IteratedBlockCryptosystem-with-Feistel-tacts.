#include "lib.h"
/*

Same to encrypt.cpp, only decrypt part and unpadding.

*/
int UnPadding(std::string & block)
{

	std::string number_str = std::string(block,0, 4);
	uint32_t len_block_zero=0;
	int errpr_flag = StringToUint32(number_str, len_block_zero);
	if (errpr_flag)
	{
		//len_block_zero ^= 0x0000cccc;
		//len_block_zero = len_block_zero >> 24;
	//	printf("\nPadding is %d ret=%d\n", len_block_zero, 4 - len_block_zero);
	//	std::cout << number_str<<std::endl;
		return (4-len_block_zero);
	}
	else printf("Error while unpadding\n");
	return 0;
  
}

bool DecryptTakeBlock(std::string buffer, uint32_t & block, uint32_t & cur_pos)
{
    std::string block_str(buffer, cur_pos, 4);

    StringToUint32(block_str, block);
    cur_pos += 4;

    if (cur_pos >= buffer.length())
    {
        return false;
    }

    return true;
}

void Decrypt_Fist(std::string buffer, std::string & cipher, uint32_t key)
{
    uint32_t cur_pos = 0;
    bool flag = true;

    while (flag)
    {
        uint32_t x, y;

        flag = DecryptTakeBlock(buffer, x, cur_pos);

		EncryptDecryptBlock_Round5(x, key);
        y = decryptBlock_Fist(x, key);
  
        std::string block;

        Uint32ToString(y, block);
		
        if (flag == false)
        {
			uint32_t var1 = cipher.length();
			uint32_t var2 = UnPadding(block);
		//	printf("%d - %d = %d\n", var1, var2, var1 - var2);
			if (var1 - var2 < 0)
			{
				printf("Eror");
				system("pause");
			}
			if (var1 == 0)
			{
				
				cipher += block;
			}
			else
			
			{
				
				
				cipher.resize(var1 - var2);
			//	cipher += block;
			}
			
        }
		else   cipher += block;
    }

    return;
}


uint32_t DecryptBlock(int32_t block, uint32_t key)
{
	uint32_t y;


	y = block;
	EncryptDecryptBlock_Round5(y, key);
	y = decryptBlock_Fist(y, key);

	return y;
}
