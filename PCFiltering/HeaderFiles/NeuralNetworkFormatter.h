
#include <stdint.h>
#include <vector>
#include "PointFeatures.h"

namespace cogs
{
	class Scan;
}

class NeuralNetworkFormatter
{
public:
	NeuralNetworkFormatter(cogs::Scan scan);
	void GenerateDataFiles();
	void GeneratePredictionFile();
private:
	std::vector<std::vector<PointFeatures>> data_;
};