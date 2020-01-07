
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
  cutter.Import("corn/corn_10.cogs");
  cutter.Trim();
  cutter.Cut(1220, 950, 1320, 1050);
  cutter.Export();
}

void formatter()
{
  DataFormatter formatter;
  int tile_size = 19;
  formatter.Import("bmw/bmw_01.cogs");
  formatter.Trim();
  formatter.GenerateImageFiles();
  formatter.GenerateDataFiles(tile_size);
  formatter.GenerateTruthFile("bmw/truth_01.cogs", tile_size);
}

void imager()
{
  DataFormatter formatter;
  formatter.Import("small/toilet_01.cogs");
  formatter.Trim();
  formatter.GenerateImageFiles();
}

void processor()
{
  PredictionProcessor processor;
  processor.ProcessPrediction("parts_02_neuron_prediction.csv", "parts/parts_02.cogs");
}

void visualizer()
{
  Visualization::Visualize("bmw", 1);
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

