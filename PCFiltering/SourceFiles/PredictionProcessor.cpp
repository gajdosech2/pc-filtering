#pragma once
#pragma warning(disable: 4996)
#define GLM_ENABLE_EXPERIMENTAL

#include "..\HeaderFiles\PredictionProcessor.h"
#include <COGS/Scan.h>
#include <vector>
#include <fstream>
#include <sstream>

const std::string DATASETS_ROOT = "DataSets/";
const std::string DATA_FILES_ROOT = "../Model/DataFiles/";

void PredictionProcessor::ProcessPrediction(std::string prediction_file_path, std::string cogs_file_path)
{
  cogs::Scan scan;
  cogs_file_path = DATASETS_ROOT + cogs_file_path;
  scan.Import(cogs_file_path);

  auto predictions = ReadPredictionFile(prediction_file_path);
  std::vector<unsigned int> ids_to_delete;
  for (const auto& [index, value] : predictions)
  {
    if (value == 0)
    {
      ids_to_delete.push_back(index);
    }
  }
  scan.Erase(ids_to_delete);
  scan.ShrinkToFit();
  std::cout << "Total points deleted: " << ids_to_delete.size() << std::endl;

  std::string export_filename = cogs_file_path + "_EXPORT.cogs";
  scan.Export(export_filename);
}

std::map<int, int> PredictionProcessor::ReadPredictionFile(std::string prediction_file_path)
{
  std::map<int, int> res;
  prediction_file_path = DATA_FILES_ROOT + prediction_file_path;
  std::ifstream prediction_file(prediction_file_path);
  if (prediction_file.is_open())
  {
    std::string line;
    while (getline(prediction_file, line))
    {
      std::istringstream stream(line);
      int index, value;
      char delimiter, end;
      stream >> index >> delimiter >> value;
      res[index] = value;
    }
    prediction_file.close();
  }
  return res;
}
