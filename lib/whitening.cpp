#include "lib.h"

/*
Implementation of whitening tact
*/
void EncryptDecryptMakeRound5(uint32_t & y, uint32_t k)
{
    y = y ^ k;
}

void EncryptDecryptBlock_Round5(/*uint32_t x, */uint32_t & y, uint32_t key)
{

    const uint16_t q1 = key >> 16;
    const uint16_t q2 = key & 0xFFFF;

	// o1^o2 + o1^o2
    const uint32_t ks5 = (((uint32_t)(q1 ^ q2)) << 16) | (q1 ^ q2);

    //y = x;

    EncryptDecryptMakeRound5(y, ks5);
}
