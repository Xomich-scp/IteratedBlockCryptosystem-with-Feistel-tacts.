#include "lib.h"
/*


Implementation of tact function.

*/
#include <vector>

#define CNT_ROUND (4)

//CHECK_S_1
std::vector<uint16_t> substitutionBox = { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7 };
//Seems Checked.
using namespace std;
// this function has the form of SP cryptosystem
uint16_t roundFunction(uint16_t subBlock, uint16_t key)
{
    uint16_t ultimateResult = 0;
	//x2 = 3331
	//key = c05b
    uint16_t localResult = subBlock ^ key;//0xf36a

    uint16_t newBitPosition;
    uint16_t currentBit;

    // 16 bit = 4 * 4 bit
    // e.g. S(1110 1010 0011 0000) = S(1110) || S(1010) || S(0011) || S(0000)
    // (it index in S)                  14         12         3          0
    // substitutionBox[14] = 3 and etc                             
    //                                  0          5          1          14
    //      S(1110 1010 0011 0000) =   0000  ||   0101  ||   0001  ||   1110 
    // substitution (S)
    localResult = (substitutionBox[(localResult >> 12) % 16] << 12) +
        (substitutionBox[(localResult >> 8) % 16] << 8) +
        (substitutionBox[(localResult >> 4) % 16] << 4) +
        (substitutionBox[(localResult >> 0) % 16] << 0);
	//71b6
	uint8_t localResult_bit[16], ultimateResult_bit[16];
    // permutation (P)
    for (int i = 0; i < 16; i++)
    {
		//CHECK_P_2
		//get new position        
		//get [i] bit
		localResult_bit[15-i]=(localResult >> (15 - i)) % 2;
		ultimateResult_bit[i] = 0;
        
    }
	/*
	for (int i = 0; i < 16; i++)
	{
		printf("%2d ", i);
	}
	printf("\n");
	*/
	for (int i = 0; i < 16; i++)
	{
		//apply P action
		newBitPosition = (13 * i + 1) % 16;
		//printf("%2d ", newBitPosition);
		ultimateResult_bit[newBitPosition] = localResult_bit[i];

	}
	/*
	printf("\n");
	for (int i = 0; i < 16; i++)
	{
		printf("%2d ", localResult_bit[i]);
	}
	printf("\n");
	for (int i = 0; i < 16; i++)
	{
		printf("%2d ", ultimateResult_bit[i]);
	}
	*/
	for (int i = 0; i < 16; i++)
	{
		//build result
		
		ultimateResult += (((uint16_t)ultimateResult_bit[i]) << (1 * i));
		

	}



    return ultimateResult;
}

#define CNT_SHEDULE_KEY (4)

void encrypt_decrypt_Block_Fist_one_round(uint32_t & block, uint32_t key, int round)
{
    uint16_t keyArray[CNT_SHEDULE_KEY] = { uint16_t(key >> 16), (uint16_t)key, uint16_t(key >> 16), (uint16_t)key };

    uint16_t leftPart  = (uint16_t)(block >> 16);
    uint16_t rightPart = (uint16_t)((block << 16)>>16);
    uint16_t roundFunctionResult;


	roundFunctionResult = roundFunction(rightPart, keyArray[round]);
	leftPart ^= roundFunctionResult;
	block = ((uint32_t)rightPart << 16) | leftPart;

}

uint32_t encryptBlock_Fist(uint32_t block, uint32_t key)
{
	uint16_t leftPart;
	uint16_t rightPart;
	uint32_t functionResult = block;
	//printf("\n Debbug\n");

    // Fist substitution (four times)
	//tau, k0, k1, k2, k3
	//CHECK_FIST_Four
    for (int i = 0; i < CNT_ROUND; i++)
    {
        encrypt_decrypt_Block_Fist_one_round(functionResult, key, i);
    }
	/*tau shift*/
	leftPart = (uint16_t)(functionResult >> 16);
	rightPart = (uint16_t)((functionResult << 16) >> 16);
	functionResult = ((uint32_t)rightPart << 16) | leftPart;

    return functionResult;
}

uint32_t decryptBlock_Fist(uint32_t block, uint32_t key)
{
   
	uint16_t leftPart;// = (uint16_t)(block >> 16);
	uint16_t rightPart;// = (uint16_t)((block << 16) >> 16);
	uint32_t functionResult = block;// = ((uint32_t)rightPart << 16) | leftPart;

    // Fist (four times)
    // Need reverse substitution
	//CHECK_FIST_Four

	//tau, k3, k2, k1, k0

    for (int i = CNT_ROUND - 1; i >= 0; i--)
    {
        encrypt_decrypt_Block_Fist_one_round(functionResult, key, i);
    }
	/*tau shit*/
	leftPart = (uint16_t)(functionResult >> 16);
	rightPart = (uint16_t)((functionResult << 16) >> 16);
	functionResult = ((uint32_t)rightPart << 16) | leftPart;

    return functionResult;
}

