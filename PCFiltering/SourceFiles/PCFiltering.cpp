
#include "..\HeaderFiles\Visualization.h"
#include "..\HeaderFiles\NeuralNetworkFormatter.h"
#include <iostream>

int main()
{
  NeuralNetworkFormatter formatter;
  formatter.Import("DataSets/toilet/toilet_01.cogs");
  formatter.Trim();
  //formatter.Pad(192);
  formatter.GenerateImageFiles();
  //formatter.GenerateDataFiles(11);
  //formatter.GenerateTruthFile("DataSets/apple/truth_02.cogs", 11);
  return 0;
}

