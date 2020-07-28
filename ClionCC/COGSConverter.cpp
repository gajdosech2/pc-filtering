#include "DataFormatter.h"
#include "PointCleanNetFormatter.h"
#include <cstring>

/*
int main(int argc, char* argv[])
{
    DataFormatter formatter;
    formatter.Import(argv[1]);

    std::string out;

    if (argc == 3 && strcmp("--trim", argv[2]) == 0)
    {
        formatter.Trim();
    }
    else if (argc == 4 && strcmp("--trim", argv[3]) == 0)
    {
        formatter.Trim();
        out = argv[2];
    }
    formatter.GenerateImageFiles(out);
    return 0;
}
 */

// PointCleanNet
int main(int argc, char* argv[])
{
    PointCleanNetFormatter formatter;
    formatter.Import(argv[1]);
    formatter.Trim();
    formatter.GenerateInputFile();

    if (argc > 2)
    {
        formatter.GenerateOutliersFile(argv[2]);
    }
}