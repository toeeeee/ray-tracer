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

    // Top to bottom; i
    for (int i = 0; i < img_height; ++i) {
        std::clog << "\rScanlines remaining: " << (img_height - i) << ' ' << std::flush;
        //Left to right; j
        for (int j = 0; j < img_width; ++j) {
            auto r = double(j) / (img_width - 1);
            auto g = double(i) / (img_height - 1);
            auto b = double(j) / (img_width - 1);

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            fout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
    std::clog << "\rDone.                 \n";
}

