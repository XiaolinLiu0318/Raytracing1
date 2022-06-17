#pragma once
#ifndef BOX_H
#define BOX_H
#include "hittable.h"
#include "hittable_list.h"
#include "rect.h"
#include "flip_face.h"

class box : public hittable
{
public:
	box() {}
	box(const vec3& p0, const vec3& p1, shared_ptr<material> ptr);

	virtual bool hit(const ray& r, double t0, double t1, hit_record& rec) const;
	virtual bool bounding_box(double t0, double t1, aabb& output_box) const {
		output_box = aabb(box_min, box_max);
		return true;
	}
public:
	vec3 box_min;
	vec3 box_max;
	hittable_list sides;
};

box::box(const vec3& p0, const vec3& p1, shared_ptr<material> ptr) {
	box_min = p0;
	box_max = p1;

	sides.add(make_shared<rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), p1.z(), ptr));
	sides.add(make_shared<flip_face>(make_shared<rect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), p0.z(), ptr)));

    sides.add(make_shared<rect>(p0.x(), p1.x(), p1.y(), p1.y(), p0.z(), p1.z(), ptr));
	//sides.add(make_shared<flip_face>(make_shared<rect>(p0.x(), p1.x(), p0.y(), p0.y(), p0.z(), p1.z(), ptr)));
	sides.add((make_shared<rect>(p0.x(), p1.x(), p0.y(), p0.y(), p0.z(), p1.z(), ptr)));

	sides.add(make_shared<rect>(p1.x(), p1.x(), p0.y(), p1.y(), p0.z(), p1.z(), ptr));
	sides.add(make_shared<flip_face>(make_shared<rect>(p0.x(), p0.x(), p0.y(), p1.y(), p0.z(), p1.z(), ptr)));
}

bool box::hit(const ray& r, double t0, double t1, hit_record& rec) const {
	return sides.hit(r, t0, t1, rec);
}
#endif // !BOX_H
