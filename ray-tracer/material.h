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
	metal(const color& albedo) : albedo(albedo) {}

	bool scatter(const ray& r_in, const hit_record& rec,
				 color& attenuation, ray& scattered)
		const override {
		auto scatter_dir = reflected(r_in.direction(), rec.normal);
		scattered = ray(rec.p, scatter_dir);
		attenuation = albedo;
		return true;
	}

private:
	color albedo;

};
#endif 

