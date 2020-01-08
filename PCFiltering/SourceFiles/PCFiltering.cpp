
#include "..\HeaderFiles\Visualization.h"
#include "..\HeaderFiles\DataFormatter.h"
#include "..\HeaderFiles\FormatterGUI.h"
#include "..\HeaderFiles\WindowCutter.h"
#include "..\HeaderFiles\PredictionProcessor.h"
#include "..\HeaderFiles\SyntheticDataGenerator.h"
#include <iostream>
#include <iomanip>
#include <sstream> 

void cutter()
{
  WindowCutter cutter;
  cutter.Import("bmw/bmw_06.cogs");
  cutter.Cut(900, 1150, 2000, 1300);
  cutter.Export();
}

void formatter()
{
  DataFormatter formatter;
  int tile_size = 19;
  formatter.Import("fruit/fruit_05.cogs");
  formatter.GenerateDataFiles(tile_size);
  formatter.GenerateTruthFile("fruit/truth_05.cogs", tile_size);
}

void imager()
{
  DataFormatter formatter;
  formatter.Import("full_bmw/bmw_13.cogs");
  formatter.Trim();
  formatter.GenerateImageFiles();
  formatter.GenerateSegmentationMask("full_bmw/truth_13.cogs");
}

void batch_imager()
{
    for (int i = 1; i <= 12; i++) {
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(2) << i;
        std::string s = ss.str();
        DataFormatter formatter;
        formatter.Import("corn/corn_" + s + ".cogs");
        formatter.Trim();
        formatter.GenerateImageFiles();
        formatter.GenerateSegmentationMask("corn/truth_corn_" + s + ".cogs");
    }
}

void processor()
{
  PredictionProcessor processor;
  processor.ProcessPrediction("fruit_05_prediction.csv", "fruit/fruit_05.cogs");
}

void visualizer()
{
  Visualization::Visualize("fruit", 6);
}

void generator()
{
  SyntheticDataGenerator::GenerateData1();
}

int main()
{
  //FormateGUI::RunNana();
  //cutter();
  //formatter();
  //imager();
  batch_imager();
  //processor();
  //visualizer();
  //generator();
  return 0;
}

