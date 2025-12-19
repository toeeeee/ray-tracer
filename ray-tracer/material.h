#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material {
public:
	virtual ~material() = default;

	virtual bool scatter(const ray& r_in, const hit_record& rec, 
						 color& attenuation, ray& scattered) const {
		return false;
	}

	// Short note:
	// What is attentuation? 
	// It's the % of light loss. We store it in a color vector to show how much of each color is lost.
};

class lambertian : public material {
public: 
	lambertian(const color& albedo) : albedo(albedo) {  }

	bool scatter(const ray& r_in, const hit_record& rec,
		color& attenuation, ray& scattered)
		const override {
		// 100% hit chance
		auto scatter_direction = rec.normal + random_unit_vector();
		scattered = ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
		
		// chance version
		/*
		double p = 1.0;
		double chance = random_double();

		if (chance < p) { // Ray is bounced
			auto scatter_direction = rec.normal + random_unit_vector();

			if (scatter_direction.near_zero()) {
		  	    scatter_direction = rec.normal;
			}

		    scattered = ray(rec.p, scatter_direction);
		    attenuation = albedo / p;
			return true;
		 }
		 else { // Ray is entirely absorbed
			return false;
		 }
		 */
	 }

private:
	color albedo; 
};


class metal : public material {
public:
	metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz) {}

	bool scatter(const ray& r_in, const hit_record& rec,
				 color& attenuation, ray& scattered)
		const override {
		vec3 scatter_dir = reflected(r_in.direction(), rec.normal); // Both dir and normal are unit vecs
		scatter_dir = unit_vector(scatter_dir) + (fuzz * random_unit_vector()); // Normalize scatter dir so fuzz sphere is consistently away from the surface by 1
		scattered = ray(rec.p, scatter_dir);
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0); // Make sure fuzzed direction is above object
	}

private:
	color albedo;
	double fuzz;
};


class dielectric : public material {
public:
	dielectric(double outer, double inner) : outer(outer), inner(inner) {}

	bool scatter(const ray& r_in, const hit_record& rec,
				 color& attenuation, ray& scattered) 
	const override {
		auto refractive_ratio = outer / inner;
		attenuation = color(1.0, 1.0, 1.0); // There is no loss of color, just warping

		// Check if ray is entering ball from outside or inside
		double rr = rec.front_face ? refractive_ratio : 1/refractive_ratio;

		// Total internal reflection
		vec3 unit_dir = unit_vector(r_in.direction());
		auto costheta = std::fmin(dot(-unit_dir, rec.normal), 1.0);
		auto sintheta = std::sqrt(1 - costheta * costheta);
		bool no_refract = rr * sintheta > 1.0;

		vec3 direction;
		if (no_refract) {
			direction = reflected(r_in.direction(), rec.normal); 
		}
		else {
			direction = refract(rr, unit_dir, rec.normal);
		}

		scattered = ray(rec.p, direction);
		return true;
	}
private:
	double outer; // Enclosing material refractive index
	double inner; // Material of object
};

#endif 

