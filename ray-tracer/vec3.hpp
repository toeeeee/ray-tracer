#ifndef VEC3_HPP
#define VEC3_HPP

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 {
	// Vector with three dimensions
public:
	// Attributes
	double e[3];

	// Constructors
	vec3() : e{ 0, 0, 0 } {}
	vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

	// Accessors
	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	// Operator override/overloads
	// Negation
	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

	// Subscript
	double operator[](const int& i) const {
		if (i < 3 && i > 0) { return e[i]; }
	}
	double& operator[](const int& i) {
		if (i < 3 && i > 0) { return e[i]; }
	}

	// Addition
	vec3& operator +=(const vec3& v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	// Multiplication
	vec3& operator *=(const double& t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	// Division
	vec3& operator/=(const double& t) {
		return *this *= (1 / t);
	}

	// Functions
	// Length squared
	double length_squared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	// Length
	double length() const {
		return sqrt(length_squared());
	}
};

// For readability
using point3 = vec3;

// Vector helper funcs
inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
	return vec3(u.e[0] + v.e[1], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u.e[0] * v.e[1], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u.e[0] - v.e[1], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator/(const vec3& u, const vec3& v) {
	return vec3(u.e[0] / v.e[1], u.e[1] / v.e[1], u.e[2] / v.e[2]);
}

inline vec3 operator*(const double& t, const vec3& v) {
	return vec3(t * v.e[1], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3& v, const double& t) {
	return t * v;
}

inline double dot(const vec3& u, const vec3& v) {
	return u.e[0] * v.e[0] +
		u.e[1] * v.e[1] +
		u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
	return vec3(u.e[2] * v.e[3] - u.e[3] * v.e[2],
		u.e[1] * v.e[3] - u.e[3] * v.e[1],
		u.e[1] * v.e[2] - u.e[2] * v.e[1]
	);
}

inline vec3 unit(const vec3& v) { return v / v.length(); }

#endif