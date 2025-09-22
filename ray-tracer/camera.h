#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <fstream>
#include "hittable.h"

std::ofstream fout("img.ppm");

class camera {
public:
    double aspect_ratio = 1.0; // over height
    int image_width = 100; // in pixel cnt
    int samples_per_pix = 10;
    int max_depth = 10;


	void render(const hittable& world) {
        initialize();

        fout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++) {
                auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
                auto ray_direction = pixel_center - center;
                ray r(center, ray_direction);

                color pixel_color = color(0, 0, 0);
                for (int k = 0; k < samples_per_pix; k++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }

                write_color(fout, pixel_sample_scale*pixel_color);
            }
        }

        std::clog << "\rDone.                 \n";
	}

private:
    int image_height; // in px
    point3 center; // coordinates of camera center
    point3 pixel00_loc; // pixel 0,0 in image .... top left
    vec3 pixel_delta_u; // right offset
    vec3 pixel_delta_v; // down offset
    double pixel_sample_scale; // scaling factor for pixel color otherwise color wouldn't make sense

	void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;
        pixel_sample_scale = 1.0 / samples_per_pix;

        center = point3(0, 0, 0);

        // Viewport
        auto focal_length = 1.0; // -z is into the camera, +y is down, x is right
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (double(image_width) / image_height);

        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // upper left pixel
        auto viewport_upper_left = 
            center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
         pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
	}

    ray get_ray(int i, int j) const {
        // Get a ray that points to a random sample point around pixel (i,j)
        
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc +
            pixel_delta_u * (i + offset.x()) +
            pixel_delta_v * (j + offset.y());

        auto ray_direction = pixel_sample - center;
        return ray(center, ray_direction);
    }

    vec3 sample_square() const { // Note: look into other sampling methods
        // Vector to random point in a square region centered at the pixel that extends halfway to the 4 neighbor pixels
        // [-.5 to .5, -.5 to .5] unit square
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    color ray_color(const ray& r, int depth, const hittable& world) {
        if (depth < 0) {
            return color(0,0,0)
        }

        hit_record rec;
        if (world.hit(r, interval(0, infinity), rec)) {
            vec3 direction = random_on_hemisphere(rec.normal);
            // Recursively bounce the ray for a diffusive material... 
            ray bounce = ray(rec.p, direction);
            return 0.5 * ray_color(bounce, depth-1, world); // Each bounce means a loss of 50% of color
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }
};

#endif // !CAMERA_H

