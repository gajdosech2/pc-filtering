
#include "..\HeaderFiles\Visualization.h"
#include "..\HeaderFiles\NeuralNetworkFormatter.h"
#include <iostream>

int main()
{
  NeuralNetworkFormatter formatter;
  formatter.Import("DataSets/toilet/toilet_16.cogs");
  formatter.Trim();
  //formatter.Pad(256);
  //formatter.GenerateBinaryMap();
  formatter.GenerateGrayMap();
  formatter.GenerateNormalMap();
  //formatter.GenerateDataFiles(5);
  //formatter.GenerateTruthFile("DataSets/apple/truth_02.cogs");
  return 0;
}

