#include "color.hpp"
#include "vec3.hpp"

#include <iostream>
#include <fstream>

int main()
{
    // Image
    int img_width = 256;
    int img_height = 256;

    // Render
    // PPM Format
    std::ofstream fout("img.ppm"); // File name

    fout << "P3\n" << img_width << ' ' << img_height << "\n255\n";

    // Top to bottom; i
    for (int i = 0; i < img_height; ++i) {
        std::clog << "\rScanlines remaining: " << (img_height - i) << ' ' << std::flush;
        //Left to right; j
        for (int j = 0; j < img_width; ++j) {
            auto pixel_color = color(double(j) / (img_width - 1), 
                                     double(i) / (img_height - 1),
                                     double(j) / (img_width - 1));
            write_color(fout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}

