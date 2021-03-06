#include <iostream>
#include "ScanImager.h"
#include "ScanSegmentation.h"
#include "ScanBoxes.h"
#include "PointCleanNetFormatter.h"
#include "PCDFormatter.h"
#include <COGS/Scan.h>

void Generate(int argc, char* argv[], ScanFormatter *formatter, bool trim = true)
{
    // ./CC --MODE INPUT_FILE EXPORT_PATH GROUND_TRUTH_FILE 
    formatter->Import(argv[2], trim);
    std::string out = "";
    switch (argc)
    {
    case 4:
        out = argv[3];
        break;
    case 5:
        out = argv[3];
        formatter->GenerateTruth(argv[4], out);
        break;
    }
    formatter->GenerateInput(out);
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
        ScanImager formatter;
        Generate(argc, argv, &formatter);
    }
    else if ((std::string)argv[1] == "--segment")
    {
        ScanSegmentation formatter;
        Generate(argc, argv, &formatter);
    }
    else if ((std::string)argv[1] == "--boxes")
    {
        ScanBoxes formatter;
        Generate(argc, argv, &formatter, false);
    }
    else if ((std::string)argv[1] == "--xyz")
    {
        PointCleanNetFormatter formatter;
        Generate(argc, argv, &formatter);
    }
    else if ((std::string)argv[1] == "--pcd")
    {
        PCDFormatter formatter;
        Generate(argc, argv, &formatter, false);
    }
    else if ((std::string)argv[1] == "--process")
    {
        ProcessMask(argc, argv);
    }
    return 0;
}