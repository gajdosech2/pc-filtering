#include "ScanBoxes.h"
#include "FormattingUtilities.h"
#include "lodepng.h"

#include <fstream>

struct Box
{
public:
    unsigned x1, y1, x2, y2;

    Box()
    {
        x1 = std::numeric_limits<unsigned>::max();
        y1 = std::numeric_limits<unsigned>::max();
        x2 = std::numeric_limits<unsigned>::min();
        y2 = std::numeric_limits<unsigned>::min();
    }

    int GetCenterX()
    {
        return x1 + GetWidth()/2;
    }

    int GetCenterY()
    {
        return y1 + GetHeight()/2;
    }

    int GetWidth()
    {
        return x2 - x1;
    }

    int GetHeight()
    {
        return y2 - y1;
    }
};

void ScanBoxes::GenerateInput(std::string out_path)
{
    data_.TransformToSpace(utils::COGS_CAMERA_SPACE);

    GenerateNormalMap(out_path);
    GenerateDepthMap(out_path);
    GenerateCombinedMap(out_path);
}


void ScanBoxes::GenerateTruth(std::string truth_path, std::string out_path)
{
    std::string labels_file = truth_path + ((truth_path == "") ? "" : "/") + file_name_ + "_labels.png";
    std::string labels_info = truth_path + ((truth_path == "") ? "" : "/") + file_name_ + "_labels_info.txt";
   
    std::string line;
    std::ifstream info_file(labels_info);
    int lines_count = -2;

    if (info_file.is_open())
    {
        while (std::getline(info_file, line))
        {
            lines_count++;
        }
        info_file.close();
    }

    std::vector<Box> boxes;
    for (int i = 0; i < lines_count; i++)
    {
        boxes.push_back(Box());
    }

    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, labels_file);
    if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

    for (unsigned y = 0; y < height; y++)
    {
        for (unsigned x = 0; x < width; x++)
        {
            auto val = image[4 * width * y + 4 * x + 0];
            if (val > 1)
            {
                boxes[val - 2].x1 = std::min(x, boxes[val - 2].x1);
                boxes[val - 2].y1 = std::min(y, boxes[val - 2].y1);
                boxes[val - 2].x2 = std::max(x, boxes[val - 2].x2);
                boxes[val - 2].y2 = std::max(y, boxes[val - 2].y2);
            }
        }
    }

    std::ofstream out_file(out_path + file_name_ + ".txt");
    if (out_file.is_open())
    {
        for (Box& box : boxes)
        {
            out_file << box.GetCenterX() << "," << box.GetCenterY() << "," << box.GetWidth() << "," << box.GetHeight() << ",0\n";
        }
        out_file.close();
    }
}
