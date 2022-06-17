#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"
#include "bounding_box.h"

//获取球型纹理u,v坐标
void get_sphere_uv(const vec3& p, double& u, double& v)
{
	auto phi = atan2(p.z(), p.x());
	auto theta = asin(p.y());
	u = 1 - (phi + pi) / (2 * pi);
	v = (theta + pi / 2) / pi;

}
class sphere : public hittable {
public:
	sphere(){ }
	sphere(point3 cen, double r,shared_ptr<material> m) : center(cen), radius(r) ,mat_ptr(m){}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const override;
	virtual bool bounding_box(double t0, double t1, aabb& box) const override;
	point3 center;
	double radius;
	shared_ptr<material> mat_ptr;
};

class moving_sphere : public hittable {
public:
	moving_sphere() {}
	moving_sphere(
		vec3 cen0, vec3 cen1, double t0, double t1, double r, shared_ptr<material> m) : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m) {};
	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
	virtual bool bounding_box(double t0, double t1, aabb& box) const override;
	vec3 center(double time) const;
public:
	vec3 center0, center1;
	double time0, time1;
	double radius;
	shared_ptr<material> mat_ptr;
;};
vec3 moving_sphere::center(double time) const {
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0) ;
}



bool moving_sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	vec3 oc = r.origin() - center(r.time());
	double a = r.direction().length_squared();
	double half_b = dot(r.direction(), oc);
	double c = oc.length_squared() - radius * radius;
	double delta = half_b * half_b - a * c;
	double t1 = (-half_b - sqrt(delta)) / a;
	double t2 = (-half_b + sqrt(delta)) / a;

	if (t1 > t_min && t1 < t_max)
	{
		rec.t = t1;
		rec.p = r.at(t1);
		vec3 out_normal = unit_vector(rec.p - center(r.time()));
		rec.set_face_normal(r, out_normal);
		rec.mat_ptr = mat_ptr;
		get_sphere_uv((rec.p - center(time0)) / radius, rec.u, rec.v);//获取纹理u,v坐标
		return true;
	}
	if (t2 > t_min && t2 < t_max)
	{
		rec.t = t2;
		rec.p = r.at(t2);
		vec3 out_normal = unit_vector(rec.p - center(r.time()));
		rec.set_face_normal(r, out_normal);
		rec.mat_ptr = mat_ptr;
		get_sphere_uv((rec.p - center(time0)) / radius, rec.u, rec.v);//获取纹理u,v坐标
		return true;
	}
	return false;
}

bool moving_sphere::bounding_box(double t0, double t1, aabb& box) const
{
	aabb box0(center(t0) - vec3(radius, radius, radius), center(t0) + vec3(radius, radius, radius));
	aabb box1(center(t1) - vec3(radius, radius, radius), center(t1) + vec3(radius, radius, radius));
	box = surrouding_box(box0, box1);
	return true;

}




bool sphere::bounding_box(double t0, double t1, aabb& box) const
{
	box = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
	return true;
}




bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	vec3 oc = r.origin() - center;
	double a = r.direction().length_squared();
	double half_b = dot(r.direction(), oc);
	double c = oc.length_squared() - radius * radius;
	double delta = half_b * half_b - a * c;
	double t1 = (-half_b - sqrt(delta)) / a;
	double t2 = (-half_b + sqrt(delta)) / a;
	if (t1 > t_min && t1 < t_max)
	{
		rec.t = t1;
		rec.p = r.at(t1);
		vec3 out_normal = unit_vector(rec.p - center);
		rec.set_face_normal(r, out_normal);
		rec.mat_ptr = mat_ptr;
		get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);//获取纹理u,v坐标
		return true;
	}
	if (t2 > t_min && t2 < t_max)
	{
		rec.t = t2;
		rec.p = r.at(t2);
		vec3 out_normal = unit_vector(rec.p - center);
		rec.set_face_normal(r, out_normal);
		rec.mat_ptr = mat_ptr;
		get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);//获取纹理u,v坐标
		return true;
	}

	return false;
	

}


#endif