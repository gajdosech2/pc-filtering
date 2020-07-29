#include "DataFormatter.h"
#include "PointCleanNetFormatter.h"
#include <cstring>

int main(int argc, char* argv[])
{
    // ./CC INPUT_FILE GROUND_TRUTH_FILE EXPORT_PATH
    DataFormatter formatter;
    formatter.Import(argv[1]);
    formatter.Trim();
    std::string out;
    switch(argc)
    {
        case 2:
            break;
        case 3:
            formatter.GenerateSegmentationMask(argv[2]);
            break;
        case 4:
            out = argv[3];
            formatter.GenerateSegmentationMask(argv[2], out);
            break;
    }
    formatter.GenerateImageFiles(out);
    return 0;
}

/*
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
 */