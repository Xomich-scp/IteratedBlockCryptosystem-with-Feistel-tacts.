#include "lib.h"


int main(int argc, char* argv[])
{
	//system("pause");
	if (argc != 6)
	{
		Error("Need args: open_text_file key_file encrypted_file\n"
			"e.g. \"lab_04_OFB.exe -enc open.txt key.txt iv.txt OFB_encrypted.txt\"\n"
			"\"lab_04_OFB.exe -dec OFB_encrypted.txt key.txt iv.txt OFB_decrypted.txt\"");
		return 0;
	}

	std::string buffer;

	if (!ReadFile(argv[2], buffer))
	{
		Error("Bad input file");
		return 0;
	}

	uint32_t key;
	std::string key_str;

	if (!ReadFile(argv[3], key_str))
	{
		Error("Bad key file");
		return 0;
	}

	if (!StringToUint32(key_str, key))
	{
		Error("Bad key");
		return 0;
	}

	uint32_t iv; // initialization vector (sinhroposylka)
	std::string iv_str;

	if (!ReadFile(argv[4], iv_str))
	{
		Error("Bad IV file");
		return 0;
	}

	if (!StringToUint32(iv_str, iv))
	{
		Error("Bad IV");
		return 0;
	}


	std::string cipher;

	if (!strcmp(argv[1], "-enc"))
	{
		Encrypt_OFB(buffer, cipher, key, iv);
	}
	else if (!strcmp(argv[1], "-dec"))
	{
		Decrypt_OFB(buffer, cipher, key, iv);
	}
	else
	{
		Error("Invalid param");
		return 0;
	}


	WriteFile(argv[5], cipher);

	return 0;
}
