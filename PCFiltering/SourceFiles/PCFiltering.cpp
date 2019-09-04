
#include "..\HeaderFiles\Visualization.h"
#include "..\HeaderFiles\NeuralNetworkFormatter.h"
#include <iostream>

int main()
{
  NeuralNetworkFormatter formatter;
  formatter.Import("DataSets/apple/apple_02.cogs");
  formatter.Trim();
  formatter.Pad(512);
  formatter.GenerateBinaryMap();
  formatter.GenerateGrayMap();
  formatter.GenerateNormalMap();
  return 0;
}

