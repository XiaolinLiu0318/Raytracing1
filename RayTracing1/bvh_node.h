#pragma once
#ifndef BVH_NODE_H
#define BVH_NODE_H


#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"
#include "bounding_box.h"
#include <algorithm>
#include <vector>
inline bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis) {
	aabb box_a;
	aabb box_b;
	if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
		std::cerr << "No bounding box in bvh_node constructor .\n";
	return box_a.min()[axis] < box_b.min().e[axis];

}

bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
	return box_compare(a, b, 0);
}
bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
	return box_compare(a, b, 1);
}
bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
	return box_compare(a, b, 2);
}
class bvh_node : public hittable
{
public:
	bvh_node() { }
	bvh_node(hittable_list& objects, size_t start, size_t end, double t0, double t1 )
	{	
		
		int axis = random_int(0, 2);
		
		std::vector<bool(*)(const shared_ptr<hittable> , const shared_ptr<hittable>)> compare(3);
		

		compare[0] = box_x_compare;
		compare[1] = box_y_compare;
		compare[2] = box_z_compare;

		size_t object_span = end - start;
		if (object_span == 1) {
			left = right = objects.objects[start];
		}
		
		else if (object_span == 2) {
			if (compare[axis](objects.objects[start], objects.objects[start + 1])) 
			{
				left = objects.objects[start];
				right = objects.objects[start + 1];
			}
			else
			{
				left = objects.objects[start + 1];
				right = objects.objects[start];
			}
		}
		else {

			std::sort(objects.objects.begin() + start, objects.objects.begin() + end, compare[axis]);
			auto mid = start + object_span / 2;
			left = make_shared<bvh_node>(objects, start, mid, t0, t1);
			right = make_shared<bvh_node>(objects, mid, end, t0, t1);

		}
		aabb box_left, box_right;
		if(!left->bounding_box(t0,t1,box_left) || !right->bounding_box(t0,t1,box_right))
			std::cerr <<" No bounding box in bvh_node constructor .\n";
		box = surrouding_box(box_left, box_right);
	}

	shared_ptr<hittable> left;
	shared_ptr<hittable> right;
	aabb box;

	bool bounding_box(double t0, double t1, aabb& b) const
	{
		b = box;
		return true;
	}
	bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const
	{
		if (!box.hit(r, t_min, t_max))
			return false;

		bool hit_left = left->hit(r, t_min, t_max, rec);
		bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

		return hit_left || hit_right;
	}

	
};

#endif // !BVH_NODE_H