#pragma once
#ifndef ROTATE_Y_H
#define ROTATE_Y_H
#include "hittable.h"

//void rotate()
class rotate : public hittable {
public:
	rotate(shared_ptr<hittable> p, double angle,int axis);
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const;
	virtual bool bounding_box(double t0, double t1, aabb& output_box) const {
		output_box = bbox;
		return hasbox;
	}

public:
	shared_ptr<hittable> ptr;
	double sin_theta;
	double cos_theta;
	bool hasbox;
	aabb bbox;
	int idx;
};

rotate::rotate(shared_ptr<hittable> p, double angle,int axis) : ptr(p) {
	auto radians = degress_to_radians(angle);
	idx = axis;
	sin_theta = sin(radians);
	cos_theta = cos(radians);
	hasbox = ptr->bounding_box(0, 1, bbox);
	double coord[3];
	vec3 min(infinity, infinity, infinity);
	vec3 max(-infinity, -infinity, -infinity);

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {  


				coord[0] = i * bbox.max().x() + (1 - i) * bbox.min().x();
				coord[1] = j * bbox.max().y() + (1 - j) * bbox.min().y();
				coord[2] = k * bbox.max().z() + (1 - k) * bbox.min().z(); //±éÀú±ä»»8¸ö¶¥µã


				//int idx = 1; // 0:ÈÆxÖá£¬1:ÈÆyÖá£¬2£¬ÈÆzÖá;
				int idx1 = ffmin((idx+1)%3,(idx+2)%3);
				int idx2 = ffmax((idx+1)%3,(idx+2)%3);
				auto newx = cos_theta * coord[idx1] - pow(-1,idx) * sin_theta * coord[idx2];
				auto newy = pow(-1,idx) * sin_theta * coord[idx1] + cos_theta * coord[idx2];
				vec3 tester;
				tester[idx1] = newx;
				tester[idx2] = newy;
				tester[idx] = coord[idx];
				for (int c = 0; c < 3; c++)  //Ã¿¸ö¶¥µã¶¼¸üÐÂbounding_box
				{
					min[c] = ffmin(min[c], tester[c]);
					max[c] = ffmax(max[c], tester[c]);
				}  
			}
		}
	}
	bbox = aabb(min, max);
}

bool rotate::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	vec3 origin = r.origin();
	vec3 direction = r.direction();
	//int idx = 1; // 0:ÈÆxÖá£¬1:ÈÆyÖá£¬2£¬ÈÆzÖá;
	int idx1 = ffmin((idx + 1) % 3, (idx + 2) % 3);
	int idx2 = ffmax((idx + 1) % 3, (idx + 2) % 3);
	origin[idx1] = cos_theta * r.origin()[idx1] + pow(-1, idx) * sin_theta * r.origin()[idx2];
	origin[idx2] = -pow(-1, idx) * sin_theta * r.origin()[idx1] + cos_theta * r.origin()[idx2];
	

	direction[idx1] = cos_theta * r.direction()[idx1] + pow(-1, idx) * sin_theta * r.direction()[idx2];
	direction[idx2] = -pow(-1, idx) * sin_theta * r.direction()[idx1] + cos_theta * r.direction()[idx2];

	ray rotated_r(origin, direction, r.time());

	if (!ptr->hit(rotated_r, t_min, t_max, rec))
		return false;

	vec3 p = rec.p;
	vec3 normal = rec.normal;

	
	p[idx1] = cos_theta * rec.p[idx1] - pow(-1, idx) * sin_theta * rec.p[idx2];
	p[idx2] = pow(-1, idx) * sin_theta * rec.p[idx1] + cos_theta * rec.p[idx2];

	normal[idx1] = cos_theta * rec.normal[idx1] - pow(-1, idx) * sin_theta * rec.normal[idx2];
	normal[idx2] = pow(-1, idx) * sin_theta * rec.normal[idx1] + cos_theta * rec.normal[idx2];

	rec.p = p;
	rec.set_face_normal(rotated_r, normal);
	return true;
}

#endif // !ROTATE_Y_H
