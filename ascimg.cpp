#include <opencv2/opencv.hpp>
#include <iostream>
#include <format>
#include <map>
#include <cmath>
#include <cstring>
#include <limits>
#include <vector>
#include <string>
#include <sstream>

uint8_t grayscale(const cv::Vec3b& pixel) {
    return static_cast<uint8_t>(0.299 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0]);
}

std::string findChar(const std::map<std::string, int>& density, int mono_pixel) {
    std::pair<std::string, int> best = {std::string(" "), 0};
    int min_diff = std::numeric_limits<int>::max();

    for (const auto& [ch, val] : density) {
        int diff = std::abs(mono_pixel - val);
        if (diff < min_diff) {
            min_diff = diff;
            best = {ch, val};
        }
    }
    return best.first;
}

int main(int argc, char *argv[]) {
    int w = 20;
    int h = 20;

    std::string im_path;

    std::map<std::string, int> density = {
      {"Ñ", 255},
      {"@", 245},
      {"#", 235},
      {"W", 225}, 
      {"$", 215}, 
      {"9", 205}, 
      {"8", 195}, 
      {"7", 185}, 
      {"6", 175}, 
      {"5", 165}, 
      {"4", 155}, 
      {"3", 145}, 
      {"2", 135}, 
      {"1", 125}, 
      {"0", 115}, 
      {"?", 105}, 
      {"!", 95}, 
      {"a", 85}, 
      {"b", 75}, 
      {";", 65},
      {":", 55},
      {"+", 50},
      {"=", 45},
      {"-", 40},
      {"*", 35},
      {",", 20},
      {".", 10},
      {" ", 5},
    };

    for (int i = 1; i < argc; i++) {
        auto arg = argv[i];

        if ((std::strcmp(arg, "-s") == 0 || std::strcmp(arg, "--size") == 0) && i + 1 < argc) {
            std::string arg_content = argv[i + 1];

            if (arg_content.find(',') != std::string::npos) {
                std::stringstream argstream(arg_content);
                std::string segment;
                std::vector<int> result;

                while(std::getline(argstream, segment, ','))
                {
                    try {
                        result.push_back(std::stoi(segment));
                    } catch (const std::invalid_argument& e) {
                        std::cerr << "Invalid argument: cannot convert '" << segment << "' to int\n";
                    } catch (const std::out_of_range& e) {
                        std::cerr << "Out of range: '" << segment << "' is too large or small to convert\n";
                    }

                }

                w = result[0];
                h = result[1];
            }
            else {
                try {
                    w = std::stoi(arg_content);
                    h = std::stoi(arg_content);
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid argument: cannot convert '" << w << "' or '" << h << "' to int\n";
                } catch (const std::out_of_range& e) {
                    std::cerr << "Out of range: '" << w << "' or '" << h << "' is too large or small to convert\n";
                }

            }

            i++;
        }

        else if (std::strcmp(arg, "-h") == 0 || std::strcmp(arg, "--help") == 0) {
            std::cout << std::format("usage: {} [-h] [-s SIZE] [-nt] file", argv[0]) << std::endl;
            std::cout << "    -h,  --help            Display this help screen." << std::endl;
            std::cout << "    -s,  --size            The size of the output image (int or int,int)." << std::endl;
            std::cout << "    -nt, --no-transparency Disable transparency in image." << std::endl;

            return 0;
        }

        else if (std::strcmp(arg, "-nt") == 0 || std::strcmp(arg, "--no-transparency") == 0) {
            density = {
                {"Ñ", 255},
                {"@", 245},
                {"#", 235},
                {"W", 225}, 
                {"$", 215}, 
                {"9", 205}, 
                {"8", 195}, 
                {"7", 185}, 
                {"6", 175}, 
                {"5", 165}, 
                {"4", 155}, 
                {"3", 145}, 
                {"2", 135}, 
                {"1", 125}, 
                {"0", 115}, 
                {"?", 105}, 
                {"!", 95}, 
                {"a", 85}, 
                {"b", 75}, 
                {"c", 65},
                {";", 55},
                {":", 50},
                {"+", 45},
                {"=", 40},
                {"-", 35},
                {"*", 20},
                {",", 10},
                {".", 5},
            };

        }

        else {
            im_path = argv[i];
        }
    }

    if (im_path.empty()) {
        std::cerr << std::format("usage: {} [-h] [-s SIZE] [-nt] file", argv[0]) << std::endl;
        std::cerr << std::format("{}: error: the following arguments are required: file", argv[0]) << std::endl;

        return 1;
    }

    cv::Mat image = cv::imread(im_path);

    if (image.empty()) {
        std::cerr << std::format("Failed to load image '{}'.", im_path) << std::endl;
        return 1;
    }

    cv::resize(image, image, cv::Size(w * 2, h));

    int rows = image.rows;
    int cols = image.cols;
    int channels = image.channels();

    std::string ascii;

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            if (channels == 1) {
                uchar pixel = image.at<uchar>(y, x);

                auto d = findChar(density, (int)pixel);

                ascii += d;

            } else if (channels == 3) {
                cv::Vec3b pixel = image.at<cv::Vec3b>(y, x);

                auto d = findChar(density, (int)grayscale(pixel));
                int b = pixel[0];
                int g = pixel[1];
                int r = pixel[2];

                ascii += std::format("\x1b[1m\x1b[38;2;{};{};{}m{}\x1b[0m", r, g, b, d);
            }
        }

        ascii += "\n";
    }

    ascii.erase(ascii.size() - 1);

    std::cout << ascii.c_str() << std::endl;

    return 0;
}

