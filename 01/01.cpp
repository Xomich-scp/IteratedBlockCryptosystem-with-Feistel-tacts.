#include "lib.h"

// 01 program for encrypt file 
// > 01.exe open.txt key.txt encrypted.txt

int main(int argc, char * argv[])
{
    if (argc != 4)
    {
        Error("Need args: open_text_file key_file encrypted_file\n"
            "e.g. \"01.exe open.txt key.txt encrypted.txt\"");
        return 0;
    }

    std::string buffer;
	//system("pause");
    if (!ReadFile(argv[1], buffer))
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

    std::string cipher;

    Encrypt_Fist(buffer, cipher, key);

    WriteFile(argv[3], cipher);

    return 0;
}
