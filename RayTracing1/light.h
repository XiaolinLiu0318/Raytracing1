#pragma once
#ifndef LIGHT_H
#define LIGHT_H
#include "material.h"

class diffuse_light :public material {
public:
	diffuse_light(shared_ptr<texture> a) : emit(a){}

	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
	{
		return false;
	}

	virtual vec3 emitted(double u, double v, const vec3& p) const
	{
		return emit->value(u, v, p);
	}

public:
	shared_ptr<texture> emit;
};

#endif // !LIGHT_H
