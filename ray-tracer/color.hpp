#ifndef COLOR_H
#define COLOR_H

#include "vec3.hpp"

#include <iostream>

using color = vec3;

void write_color(std::ostream& out, color pixel_color) {
	out << static_cast<int>(255.99 * pixel_color.x()) << ' ';
	out << static_cast<int>(255.99 * pixel_color.y()) << ' ';
	out << static_cast<int>(255.99 * pixel_color.z()) << '\n';
}

#endif
