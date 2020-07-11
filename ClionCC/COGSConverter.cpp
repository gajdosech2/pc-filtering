#include "DataFormatter.h"
#include <cstring>

int main(int argc, char* argv[])
{
    DataFormatter formatter;
    formatter.Import(argv[1]);
    if (argc > 2 && strcmp("--trim", argv[2]) == 0)
    {
        formatter.Trim();
    }
    formatter.GenerateImageFiles();
    return 0;
}