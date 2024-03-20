#include "color.hpp"
#include "ray.hpp"
#include "vec3.hpp"

#include <iostream>
#include <fstream>

// Look at book for math on this
// Center is of sphere
// Test git 
bool hit_sphere(const point3& center, double radius, const ray& r) {
    double a = dot( r.direction(), r.direction() );
    double b = 2 * dot( r.direction(), (r.origin() - center) );
    double c = dot( (r.origin() - center), (r.origin() - center) ) - radius * radius;
    double discriminant = b*b - 4*a*c;
    return (discriminant >= 0);
}

color ray_color(const ray& r) { 
    if (hit_sphere(point3(0, 0, -1), 0.5, r)) {
        return color(1, 0, 1);
    }

    vec3 unit_dir = unit_vector(r.direction());
    auto a = 0.5 * (unit_dir.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

int main()
{
    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int img_width = 400;
    
    int img_height = static_cast<int>(img_width / aspect_ratio);
    img_height = (img_height < 1) ? 1 : img_height;

    // Camera

    auto focal_length = 1.0;
    auto viewport_height = 2.0; // Arbitary
    double actual_aspect_ratio = static_cast<double>(img_width)/img_height;
    auto viewport_width = viewport_height * actual_aspect_ratio;
    auto camera_center = point3(0, 0, 0);

    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    auto pixel_delta_u = viewport_u / img_width;
    auto pixel_delta_v = viewport_v / img_height;

    auto viewport_upper_left = camera_center -
                               - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);



    // Render
    // PPM Format
    std::ofstream fout("img.ppm"); // File name

    fout << "P3\n" << img_width << ' ' << img_height << "\n255\n";

    // Top to bottom
    for (int j = 0; j < img_height; ++j) {
        std::clog << "\rScanlines remaining: " << (img_height - j) << ' ' << std::flush;
        //Left to right
        for (int i = 0; i < img_width; ++i) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(fout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}

