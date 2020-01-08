
#include "..\HeaderFiles\Visualization.h"
#include "..\HeaderFiles\DataFormatter.h"
#include "..\HeaderFiles\FormatterGUI.h"
#include "..\HeaderFiles\WindowCutter.h"
#include "..\HeaderFiles\PredictionProcessor.h"
#include "..\HeaderFiles\SyntheticDataGenerator.h"
#include <iostream>

void cutter()
{
  WindowCutter cutter;
  cutter.Import("fruit/truth_02.cogs");
  cutter.Cut(1200, 1000, 2000, 1500);
  //cutter.Trim();
  cutter.Export();
}

void formatter()
{
  DataFormatter formatter;
  int tile_size = 19;
  formatter.Import("fruit/fruit_01.cogs");
  formatter.Trim();
  //formatter.GenerateImageFiles();
  formatter.GenerateDataFiles(tile_size);
  formatter.GenerateTruthFile("fruit/truth_01.cogs", tile_size);
}

void imager()
{
  DataFormatter formatter;
  formatter.Import("fruit/fruit_02EXPORT.cogs");
  //formatter.Trim();
  formatter.GenerateImageFiles();
}

void processor()
{
  PredictionProcessor processor;
  processor.ProcessPrediction("fruit_02_truth.csv", "fruit/fruit_02.cogs");
}

void visualizer()
{
  Visualization::Visualize("fruit", 3);
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
  //processor();
  visualizer();
  //generator();
  return 0;
}

