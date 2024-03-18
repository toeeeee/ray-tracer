#ifndef RAY_HPP
#define RAY_HPP

#include "vec3.hpp"

class ray {
public:
	ray() {}

	ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

	point3 origin() const { return orig; }
	vec3 direction() const { return dir;  }

	// Returns at a point along the ray
	point3 at(double t) const {
		return orig + t*dir;
	}

private:
	point3 orig{};
	vec3 dir{};
};

#endif