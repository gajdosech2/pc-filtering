#pragma once
#include <string>

namespace cogs {
	class Scan;
}

class SyntheticDataGenerator {

public:
	static void GenerateData1();
	static void GenerateData2();

private:
	static void Export(cogs::Scan scan, std::string name);
};