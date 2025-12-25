#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <fstream>
#include "hittable.h"
#include "material.h"

std::ofstream fout("img2.ppm");

class camera {
public:
    double aspect_ratio = 1.0; // over height
    int image_width = 100; // in pixel cnt
    int samples_per_pix = 100; // Self explanatory
    int max_depth = 10; // Number of recursive ray bounces  for each ray sent out

    double vfov = 90; // Vertical view angle
    point3 lookfrom = point3(0, 0, 0);
    point3 lookat = point3(0, 0, -1);
    vec3 viewup = vec3(0, 1, 0);

    double defocus_angle = 0; // Variation of angle of rays through each pixel
    double focus_dist = 10; // Distance to perfect focus plane (NOT THE image plane)
    // Here we will assume that the focus dist is the focal length (distance to image plane)


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

    vec3 u, v, w; // u is right, v up, w into

    vec3 defocus_disk_u; // horz. disk rad
    vec3 defocus_disk_v; // vert. disk rad


	void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;
        pixel_sample_scale = 1.0 / samples_per_pix;

        center = lookfrom;

        // Coordinate system
        vec3 temp = -(lookat - lookfrom);
        w = unit_vector(temp);
        u = unit_vector(cross(viewup, w));
        v = -cross(u, w);  // Negative because w is negative depth; think right hand rule


        // Viewport
        // -z is into the camera, +y is down, x is right
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta / 2) * focus_dist; // height
        auto viewport_height = 2.0 * h;
        auto viewport_width = viewport_height * (double(image_width) / image_height);

        auto viewport_u = viewport_width * u; // Total width and height in our basis
        auto viewport_v = -viewport_height * v;

        pixel_delta_u = viewport_u / image_width; // the equiv of 1 unit in a dir in our basis
        pixel_delta_v = viewport_v / image_height;

        // upper left pixel
        auto viewport_upper_left = 
            center - (focus_dist*w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calculate camera defocus disk basis vectors
        auto theta2 = degrees_to_radians(defocus_angle/2);
        auto defocus_radius = std::tan(theta2) * focus_dist;
        defocus_disk_u = defocus_radius * u;
        defocus_disk_v = defocus_radius * v;
	}

    ray get_ray(int i, int j) const {
        // Get a ray that points to a random sample point around pixel (i,j)
        // Ray is also a sample of points from the defocus disk.
        
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc +
            pixel_delta_u * (i + offset.x()) +
            pixel_delta_v * (j + offset.y());

        point3 ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample(); 
        vec3 ray_direction = pixel_sample - ray_origin;
        return ray(ray_origin, ray_direction);
    }

    vec3 defocus_disk_sample() const {
        // Return a ray from the defocus disk of the camera
        vec3 r = random_in_unit_disk_rejection();
        return center + (r[0] * defocus_disk_u) + (r[1] * defocus_disk_v); // Change unit disk to unit disk in proper basis.
    }

    vec3 sample_square() const { // Note: look into other sampling methods
        // Vector to random point in a square region centered at the pixel that extends halfway to the 4 neighbor pixels
        // [-.5 to .5, -.5 to .5] unit square
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    color ray_color(const ray& r, int depth, const hittable& world) {
        if (depth <= 0) {
            return color(0, 0, 0);
        }

        hit_record rec;
        if (world.hit(r, interval(0.001, infinity), rec)) { // 0.001 to remove shadow acne where ray origin isn't flush with surface due to rounding errors
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                return attenuation * ray_color(scattered, depth-1, world); // Each bounce means a loss of x% of color
            }
            return color(0, 0, 0); // No scatter = absorbed and black 
        }

        // Sky
        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }
};

#endif // !CAMERA_H

