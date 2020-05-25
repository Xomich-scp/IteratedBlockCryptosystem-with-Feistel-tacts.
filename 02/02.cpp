#include "lib.h"

// 02 program for decrypt file
// > 02.exe encrypted.txt key.txt decrypted.txt

int main(int argc, char* argv[])
{
	
    if (argc != 4)
    {
        Error("Need args: encrypted_file key_file decrypted_file"
            "e.g. \"02.exe encrypted.txt key.txt decrypted.txt\"");
        return 0;
    }
	//system("pause");
    std::string cipher;

    if (!ReadFile(argv[1], cipher))
    {
        Error("Bad input file");
        return 0;
    }

    uint32_t key;
    std::string key_str;

    if (!ReadFile(argv[2], key_str))
    {
        Error("Bad key file");
        return 0;
    }

    if (!StringToUint32(key_str, key))
    {
        Error("Bad key");
        return 0;
    }

    

    std::string buffer;

    Decrypt_Fist(cipher, buffer, key);

    WriteFile(argv[3], buffer);

    return 0;
}
