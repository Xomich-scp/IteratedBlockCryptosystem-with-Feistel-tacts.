#include "lib.h"
/*

Implemntaton of Crypt part. Here we deal with padding and retrieving new block for crypting. 

For tact's operation see "Fist_Func.cpp"

*/

void Padding(std::string &block, uint32_t len_to_fit)
{	
	//printf("\nPadding = %d\n", len_to_fit);
	for (int i=0; i< len_to_fit; i++)
    block += "\x30";
	block += "\x30\x30\x30";
	block += 4-len_to_fit;
//	printf("\nPadding is %d\n",4-len_to_fit);
//	
	
}

bool EncryptTakeBlock(std::string buffer, uint32_t & block, uint32_t & cur_pos)
{
    bool flag = true;
    std::string block_str(buffer, cur_pos, 4);



    StringToUint32(block_str, block);
    cur_pos += 4;

	if (cur_pos >= buffer.length())
	{
		flag = false;
		block = (block << 24)>>24;
	}

	if ((cur_pos + 4 >= buffer.length()) && flag)
	{

		std::string number_str = std::string(buffer, cur_pos+3, 1);
		uint32_t len_block_zero = 0;
		int errpr_flag = StringToUint32(number_str, len_block_zero);
		if (errpr_flag)
		{
			len_block_zero = len_block_zero >> 24;
			//printf("\nEnc padding is %d\n", 4 - len_block_zero);
		//	std::cout << number_str << std::endl;
			block = (block >> (8 * (4-len_block_zero))) << ((4 - len_block_zero) * 8);
		}
		else printf("Error while unpadding\n");

	}
    return flag;
}

void Encrypt_Fist(std::string cipher, std::string & buffer, uint32_t key)
{
    uint32_t cur_pos = 0;
    bool flag = true;
	uint32_t padding_value = 0;
	while ((cipher.length()+ padding_value) % 4)
	{
		padding_value++;

	}
	Padding(cipher, padding_value);
	 

    do
    {
		//x for open, y for crypted
        uint32_t x, y;

		//get new block + padding
        flag = EncryptTakeBlock(cipher, x, cur_pos);

		//sp magic
        y = encryptBlock_Fist(x, key);

       
        //whitering
		EncryptDecryptBlock_Round5(y, key);

        std::string block;

        Uint32ToString(y, block);

        buffer += block;
    } while (flag);

    return;
}


uint32_t EncryptBlock(int32_t block, uint32_t key)
{
	uint32_t y;

	y = encryptBlock_Fist(block, key);
	EncryptDecryptBlock_Round5(y, key);

	return y;
}
