#define _CRT_SECURE_NO_WARNINGS

#include "lib.h"

#include <ctime>
#include <bitset>

// для одного смещения полный перебор 0xFFFF FFFF - 40 минут
// 40 минут * 32 * 4

// нужен вывод четырех раундов (первые три можно не делать)
#define START_ROUND (3)
// тест
#define START_OFFSET (7)

#define ROUND_TOTAL (4)

#define ROUND_TOTAL_WINTH_WITHENING (ROUND_TOTAL + 1)

// Convert Little Endian to Big Endian
#define CONVERT_LITTLE_BIG(x) ((x >> 24) | ((x << 8) & 0x00ff0000) | ((x >> 8) & 0x0000ff00) | (x << 24))

uint32_t log_[ROUND_TOTAL_WINTH_WITHENING] = { 0 };      // результаты тактовых преобразований для блока Х
uint32_t log_error[ROUND_TOTAL_WINTH_WITHENING] = { 0 }; // результаты тактовых преобразований для блока Х'

// Определяет писать в логи для блока обычного или для блока с ошибкой в фиксированном бите
// Потому что передаем в ecnryptBlock сначала X, потом X_error
int flag = 0;

#define PRINT_X_XOR_X \
    DisplayMsgAndUin32IntoGroupsOf4Bits("X      = ", X_def); \
    DisplayMsgAndUin32IntoGroupsOf4Bits("X'     = ", X_error_def); \
    DisplayMsgAndUin32IntoGroupsOf4Bits("X ^ X' = ", X_def ^ X_error_def);

#define PRINT_Y_XOR_Y_SUM \
    std::cout << "round = " << j + 1 << std::endl; \
    DisplayMsgAndUin32IntoGroupsOf4Bits("Y      = ", log_[j]); \
    DisplayMsgAndUin32IntoGroupsOf4Bits("Y'     = ", log_error[j]); \
    DisplayMsgAndUin32IntoGroupsOf4Bits("Y ^ Y' = ", log_[j] ^ log_error[j]); \
    changing |= log_[j] ^ log_error[j]; \
    DisplayMsgAndUin32IntoGroupsOf4Bits("Sum (Y^Y') = ", changing);

int ecnryptBlock(uint32_t & block, uint32_t key, int round_total)
{
    // ::var -- appeal to global variables
    ::flag = (::flag + 1) % 2;
    
    for (int round = 0; round < round_total; round++)
    {
        encrypt_decrypt_Block_Fist_one_round(block, key, round);
        
        (flag == 1)
            ? (::log_[round] = block) 
            : (::log_error[round] = block);
    }
    
    if (round_total == ROUND_TOTAL)
    {
        EncryptDecryptBlock_Round5(block, key);

        (flag == 1)
            ? (::log_[round_total] = block)
            : (::log_error[round_total] = block);
    }

    return 1;
}

int main()
{
    uint32_t block = 0;
    
    uint32_t X;           // открытый блок
    uint32_t X_error;     // открытый блок с ошибкой
    uint32_t X_def;       // открытый блок изначальный для вывода
    uint32_t X_error_def; // открытый блок с ошибкой изначальный для вывода
    
    uint32_t Xor[4] = { 0 };
    
    uint32_t maxNumber = 0xFFFFFFFF;
    
    uint32_t changing = 0;
    uint32_t totalBits = 0;

    GetCurrentTime();

    std::srand(unsigned(std::time(0)));

    uint32_t key = ((std::rand() << 16) + (std::rand())) & 0x7FFFFFFF;

    printf("Start value of key: 0x%.8X\n", CONVERT_LITTLE_BIG(key));

    int i = START_ROUND;
    int offset = START_OFFSET;
    int j = 0;

    for (i = START_ROUND; i < ROUND_TOTAL; i++)
    {
        std::cout << "Start round: " << i << std::endl;

        for (offset = START_OFFSET; offset < 32; offset++)
        {
            std::cout << "Start offset: " << offset << std::endl;

            Xor[i] = 0;
    
            do
            {
                if (block & (0x80000000 >> offset))
                {
                    block++;
                    continue;
                }

                // Внесли ошибку
                X = block;
                X_error = block ^ (0x80000000 >> offset);

                // Записали блоки до раундов
                X_def = X;
                X_error_def = X_error;

                ecnryptBlock(X, key, i + 1);
                ecnryptBlock(X_error, key, i + 1);

                // Сумма разниц 2-х зашифрованных блоков
                Xor[i] |= X ^ X_error;

                //totalBits |= Xor[i]; //
                
                if (Xor[i] == maxNumber)
                {
                    break;
                }

                block++;
            } while (block);

            // Если ошибка размножилась
            if (Xor[i] == maxNumber)
            {
                // Если попробовали не все оффсеты, то идем дальше
                if (offset != 31)
                {
                    continue;
                }
                
                std::cout << std::endl;
                
                std::cout << std::endl << "essential var" << std::endl;

                std::cout << "Error multiplied in round " << i + 1 << std::endl;
                std::cout << "difference[" << i << "] == 0xFFFFFFFF" << std::endl;
                std::cout << "key = " << key << std::endl;
                std::cout << std::endl;
                std::cout << "offset: " << offset << std::endl;
                
                PRINT_X_XOR_X

                for (j = 0; j < i + 1; j++)
                {
                    PRINT_Y_XOR_Y_SUM
                }

                std::cout << std::endl;
                
                return 1;
            }

            // Если не размножилась, проверять этот раунд смысла нет (тут несущественную переменную можно посмотреть)
            else
            {
                std::cout << std::endl << "nonessential var" << std::endl;

                PRINT_X_XOR_X

                for (j = 0; j < i + 1; j++)
                {    
                    PRINT_Y_XOR_Y_SUM

                    // Та самая супер переменная, наличие в которой нулей говорит о несущественной переменной 
                    DisplayMsgAndUin32IntoGroupsOf4Bits("Total bits = ", Xor[i]);
                }

                std::cout << std::endl;
                
                break;
            }
        }
    }

    std::cout << "The error did not multiply" << std::endl;

    GetCurrentTime();

    return 1;
}
