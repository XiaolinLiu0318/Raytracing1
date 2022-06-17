#pragma once
#ifndef RECT_H
#define RECT_H
#include "hittable.h"
class rect :public hittable {
public:
	rect() {}
	rect(double _x0, double _x1, double _y0,double _y1, double _z0,double _z1, shared_ptr<material> mat)// bool _outward_normal = true)
	{
		coord.resize(3);
		coord[0].push_back(_x0);
		coord[0].push_back(_x1);
		coord[1].push_back(_y0);
		coord[1].push_back(_y1);
		coord[2].push_back(_z0);
		coord[2].push_back(_z1);
		idx = coord[0][0] == coord[0][1] ? 0 : coord[1][0] == coord[1][1] ? 1 : 2;
		mp = mat;
		
		//outward = _outward_normal;
		std::cout << outward << std::endl;

	};

	virtual bool hit(const ray& r, double t0, double t1, hit_record& rec) const;

	virtual bool bounding_box(double t0, double t1, aabb& output_box) const {
	
		vec3 Min = { coord[0][0], coord[1][0], coord[2][0] };
		vec3 Max = { coord[0][1], coord[1][1], coord[2][1] };
		Min[idx] -= 0.0001;
		Max[idx] += 0.0001;
		output_box = aabb(Min, Max);
		return true;
	}


public:
	shared_ptr<material> mp;
	vector<vector<double>> coord;
	int idx;
	bool outward;

};


bool rect::hit(const ray& r, double t0, double t1, hit_record& rec) const {
	
	double k = coord[idx][0];
	
	auto t = (k - r.origin()[idx]) / r.direction()[idx];
	if (t < t0 || t >t1)
		return false;

	int idx1 = (idx + 1) % 3;
	int idx2 = (idx + 2) % 3;
	auto coord1 = r.origin()[idx1] + t * r.direction()[idx1];
	auto coord2 = r.origin()[idx2] + t * r.direction()[idx2];
	if (coord1 < coord[idx1][0] || coord1 > coord[idx1][1] || coord2 < coord[idx2][0] || coord2 > coord[idx2][1])
	{
		return false;
	}

	rec.u = (coord1 - coord[idx1][0]) / (coord[idx1][1] - coord[idx1][0]);
	rec.v = (coord2 - coord[idx2][0]) / (coord[idx2][1] - coord[idx2][0]);

	rec.t = t;
	vec3 outward_normal = vec3(0, 0, 0);
	
	outward_normal[idx] += 1;
	rec.set_face_normal(r, outward_normal);
	
	//if (!outward)
	//{
	//	//rec.front_face = !rec.front_face;
	//	rec.normal = -rec.normal;
	//}
	
	
	rec.mat_ptr = mp;
	rec.p = r.at(t);
	return true;

}


#endif // !RECT_H
