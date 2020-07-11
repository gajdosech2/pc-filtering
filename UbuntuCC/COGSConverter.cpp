#include "DataFormatter.h"

int main(int argc, char* argv[])
{
    DataFormatter formatter;
    formatter.Import(argv[1]);
    formatter.GenerateImageFiles();
    return 0;
}