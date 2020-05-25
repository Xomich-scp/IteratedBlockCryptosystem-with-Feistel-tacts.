#pragma once

#include <iostream>
#include <fstream>



// OFB_enc_dec.cpp
void Decrypt_OFB(std::string buffer, std::string& cipher, uint32_t key, uint32_t iv);
void Encrypt_OFB(std::string cipher, std::string& buffer, uint32_t key, uint32_t iv);


// encrypt.cpp
// uint32_t len_to_fit);
void Padding(std::string & block, uint32_t len_to_fit);
bool EncryptTakeBlock(std::string buffer, uint32_t & block, uint32_t & cur_pos);
void Encrypt_Fist(std::string cipher, std::string & buffer, uint32_t key);
uint32_t EncryptBlock(int32_t block, uint32_t key);


// decrypt.cpp
int UnPadding(std::string & block);
bool DecryptTakeBlock(std::string buffer, uint32_t & block, uint32_t & cur_pos);
void Decrypt_Fist(std::string buffer, std::string & cipher, uint32_t key);
uint32_t DecryptBlock(int32_t block, uint32_t key);


// files.cpp
int ReadFile(char * path, std::string & buffer);
void WriteFile(char * path, std::string & buffer);

// Lai-Massey.cpp
uint16_t roundFunction(uint16_t subBlock, uint16_t key);
void encrypt_decrypt_Block_Fist_one_round(uint32_t & block, uint32_t key, int round);
uint32_t encryptBlock_Fist(uint32_t block, uint32_t key);
uint32_t decryptBlock_Fist(uint32_t block, uint32_t key);

// utils.cpp
int StringToUint32(std::string block_str, uint32_t & block);
void Uint32ToString(uint32_t & key, std::string & block_str);
void Error(char const * msg);
void GetCurrentTime();
void DisplayMsgAndUin32IntoGroupsOf4Bits(char const * msg, uint32_t number);

// whitening.cpp
void EncryptDecryptMakeRound5(uint32_t & y, uint32_t k);
//void EncryptDecryptBlock_Round5(uint32_t x, uint32_t & y, uint32_t key);
void EncryptDecryptBlock_Round5(uint32_t & y, uint32_t key);

int GetBits(int x);
void differ_table(int* Sbox);
void liner_table(int* Sbox);
