#include <iostream>
#include "ScanImager.h"
#include "ScanSegmentation.h"
#include "ScanBoxes.h"
#include "XYZFormatter.h"
#include "PCDFormatter.h"
#include <COGS/Scan.h>

void Generate(int argc, char* argv[], ScanFormatter *formatter, bool trim = true, bool transform = false)
{
    // ./CC --MODE INPUT_PATH EXPORT_PATH GROUND_TRUTH_PATH 
    formatter->Import(argv[2], trim, transform);
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

void ProcessOutput(int argc, char* argv[])
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
        Generate(argc, argv, &formatter, true, true);
    }
    else if ((std::string)argv[1] == "--segment")
    {
        ScanSegmentation formatter;
        Generate(argc, argv, &formatter, false);
    }
    else if ((std::string)argv[1] == "--boxes")
    {
        ScanBoxes formatter;
        if (argc <= 4)
        {
            Generate(argc, argv, &formatter, false);
        }
        else
        {
            formatter.class_id = atoi(argv[5]);
            Generate(5, argv, &formatter, false);
        }
    }
    else if ((std::string)argv[1] == "--xyz")
    {
        XYZFormatter formatter;
        Generate(argc, argv, &formatter);
    }
    else if ((std::string)argv[1] == "--pcd")
    {
        PCDFormatter formatter;
        Generate(argc, argv, &formatter, false);
    }
    else if ((std::string)argv[1] == "--process")
    {
        ProcessOutput(argc, argv);
    }
    return 0;
}