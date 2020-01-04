
#include "..\HeaderFiles\Visualization.h"
#include "..\HeaderFiles\NeuralNetworkFormatter.h"
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
	NeuralNetworkFormatter formatter;
	int tile_size = 15;
	formatter.Import("parts/parts_03.cogs");
	formatter.Trim();
	formatter.Pad(128);
	formatter.GenerateImageFiles();
	formatter.GenerateDataFiles(tile_size);
	formatter.GenerateTruthFile("parts/truth_03.cogs", tile_size);
}

void imager() {
	NeuralNetworkFormatter formatter;
	formatter.Import("small/toilet_01.cogs");
	formatter.Trim();
	formatter.GenerateImageFiles();
}

void processor() {
	PredictionProcessor processor;
	processor.ProcessPrediction("parts_02_neuron_prediction.csv", "parts/parts_02.cogs");
}

void visualizer() {
	Visualization::Visualize("parts", 4);
}

void generator() {
	SyntheticDataGenerator::GenerateData1();
}

int main()
{
	//FormateGUI::RunNana();
	//cutter();
	formatter();
	//imager();
	//processor();
	//visualizer();
	//generator();
	return 0;
}

