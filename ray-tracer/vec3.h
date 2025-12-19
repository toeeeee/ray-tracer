#pragma once
#ifndef VEC3_H
#define VEC3_H

#include "rtweekend.h"

class vec3 {
public:
    double e[3];

    vec3() : e{ 0,0,0 } {}
    vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(double t) {
        return *this *= 1 / t;
    }

    static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

    static vec3 random(double min, double max) {
        return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
    }

    double length() const {
        return std::sqrt(length_squared());
    }

    double length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    bool near_zero() const {
        auto s = 1e-8;
        return (std::fabs(e[0]) < s) && 
               (std::fabs(e[1]) < s) &&
               (std::fabs(e[2]) < s);
    }

};

using point3 = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator+(const vec3& u, const float f) {
    return vec3(u.e[0] + f, u.e[1] + f, u.e[2] + f);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}


inline vec3 operator-(const vec3& u, const float f) {
    return vec3(u.e[0] - f, u.e[1] - f, u.e[2] - f);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3& v) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(const vec3& v, double t) {
    return (1 / t) * v;
}

inline double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0]
        + u.e[1] * v.e[1]
        + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

inline vec3 random_unit_vector() { // Look into better ways (read: analytical) for generation
    while (true) {
        auto p = vec3::random(-1, 1);
        auto l2 = p.length_squared();
        if (1e-160 < l2 && l2 <= 1) {
            return p / sqrt(l2);
        }
    }
}

inline vec3 random_on_hemisphere(vec3 const& normal) {
    // Random vector on hemisphere along normal
    vec3 on_unit_sphere = random_unit_vector();
    return (dot(on_unit_sphere, normal) > 0) ? on_unit_sphere : -on_unit_sphere;
}

inline vec3 reflected(vec3 const& incident, vec3 const& normal) {
    return incident - 2*dot(incident, normal)*normal;
}

/* Derivation for how Snell's law is applied here can be found in the github
*/

inline vec3 refract(double refractive_ratio, vec3 const& incident, vec3 const normal) {
    vec3 refracted_perpendicular = refractive_ratio * (incident - dot(incident, normal) * normal);
    vec3 refracted_parallel = -sqrt(1 - refracted_perpendicular.length_squared()) * normal;
    return refracted_perpendicular + refracted_parallel;
}


#endif