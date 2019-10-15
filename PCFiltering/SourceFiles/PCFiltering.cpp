
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
	formatter.Import("synthetic/synthetic_03.cogs");
	formatter.Trim();
	formatter.GenerateImageFiles();
	formatter.GenerateDataFiles(tile_size);
	formatter.GenerateTruthFile("synthetic/synthetic_04.cogs", tile_size);
}

void imager() {
	NeuralNetworkFormatter formatter;
	formatter.Import("parts/truth_02.cogs");
	formatter.Trim();
	formatter.GenerateImageFiles();
}

void processor() {
	PredictionProcessor processor;
	processor.ProcessPrediction("parts_04_prediction.csv", "parts/parts_04.cogs");
}

void visualizer() {
	Visualization::Visualize("synthetic", 6);
}

void generator() {
	SyntheticDataGenerator::GenerateData2();
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

