#include "lib.h"

#include <sstream>

int ReadFile(char * path, std::string & buffer)
{
    std::ifstream file(path, std::ios_base::binary);

    if (!file.is_open())
    {
        Error("Can not open the file");
        return 0;
    }

    std::stringstream ss;

    ss << file.rdbuf();

    buffer = ss.str();

    if (buffer.length() == 0)
    {
        Error("The file is empty");
        file.close();

        return 0;
    }

   

    return 1;
}

void WriteFile(char * path, std::string & buffer)
{
    std::ofstream file(path, std::ios_base::binary);
    file.clear();

    file << buffer;
    file.close();

   
}
