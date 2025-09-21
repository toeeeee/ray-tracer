#pragma once
#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"
#include "rtweekend.h"

using color = vec3;

void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y(); // each one of these are based on a norm vector with components in [0,1[
    auto b = pixel_color.z();

    // turn [0,1] components to byte range [0,255]
    static const interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif
