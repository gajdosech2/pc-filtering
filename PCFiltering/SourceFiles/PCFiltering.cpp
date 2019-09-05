
#include "..\HeaderFiles\Visualization.h"
#include "..\HeaderFiles\NeuralNetworkFormatter.h"
#include "..\HeaderFiles\FormatterGUI.h"
#include <iostream>

void formatter() {
	NeuralNetworkFormatter formatter;
	int tile_size = 11;
	formatter.Import("toilet/toilet_01.cogs");
	formatter.Trim();
	formatter.Pad(192);
	formatter.GenerateImageFiles();
	formatter.GenerateDataFiles(tile_size);
	formatter.GenerateTruthFile("apple/truth_02.cogs", tile_size);
}

int main()
{
	FormateGUI::RunNana();
	return 0;
}

