
#include "..\HeaderFiles\Visualization.h"
#include "..\HeaderFiles\NeuralNetworkFormatter.h"
#include "..\HeaderFiles\FormatterGUI.h"
#include "..\HeaderFiles\WindowCutter.h"
#include <iostream>

void formatter()
{
  NeuralNetworkFormatter formatter;
  int tile_size = 11;
  formatter.Import("toilet/toilet_01.cogs");
  formatter.Trim();
  formatter.Pad(192);
  formatter.GenerateImageFiles();
  formatter.GenerateDataFiles(tile_size);
  formatter.GenerateTruthFile("apple/truth_02.cogs", tile_size);
}

void cutter()
{
  WindowCutter cutter;
  cutter.Import("apple/apple_12.cogs");
  cutter.Trim();
  cutter.Cut(1100, 950, 1150, 1000);
  cutter.Export();
}

int main()
{
  //FormateGUI::RunNana();
  Visualization::Visualize("apple");
  //cutter();
  return 0;
}

