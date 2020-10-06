#include <iostream>
#include "ScanImager.h"
#include "ScanSegmentation.h"

void GenerateImages(int argc, char* argv[])
{
    // ./CC --generate INPUT_FILE EXPORT_PATH GROUND_TRUTH_FILE 
    ScanImager formatter;
    formatter.Import(argv[2]);
    std::string out = "";
    switch (argc)
    {
    case 4:
        out = argv[3];
        break;
    case 5:
        out = argv[3];
        formatter.GenerateTruth(argv[4], out);
        break;
    }
    formatter.GenerateInput(out);
}

void GenerateSegmentation(int argc, char* argv[])
{
    // ./CC --segment INPUT_FILE EXPORT_PATH LABELS_FILE
    ScanSegmentation formatter;
    formatter.Import(argv[2]);
    std::string out = "";
    switch (argc)
    {
    case 4:
        out = argv[3];
        break;
    case 5:
        out = argv[3];
        formatter.GenerateTruth(argv[4], out);
        break;
    }
    formatter.GenerateInput(out);
}

void ProcessMask(int argc, char* argv[])
{
    // ./CC --process COGS_FILE SEGMENTATION_MASK EXPORT_PATH
    ScanImager formatter;
    switch (argc)
    {
    case 4:
        formatter.ProcessPrediction(argv[2], argv[3]);
        break;
    case 5:
        formatter.ProcessPrediction(argv[2], argv[3], argv[4]);
        break;
    }
}

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        return 0;
    }
    if ((std::string)argv[1] == "--generate")
    {
        GenerateImages(argc, argv);
    }
    else if ((std::string)argv[1] == "--process")
    {
        ProcessMask(argc, argv);
    }
    else if ((std::string)argv[1] == "--segment")
    {
        GenerateSegmentation(argc, argv);
    }
    return 0;
}