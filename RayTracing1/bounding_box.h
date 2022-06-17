#pragma once
#include "rtweekend.h"
#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H


class aabb
{
public:
	vec3 _min;//左下角顶点
	vec3 _max;//右上角顶点
	aabb() {}
	aabb(const vec3& a, const vec3& b) { _min = a, _max = b; }

	vec3 min() const { return _min; }
	vec3 max() const { return _max; }

	//判断射线是否在t区间
	bool hit(const ray& r, double tmin, double tmax) const;
};
bool aabb::hit(const ray& r, double tmin, double tmax) const
{
	for (int a = 0; a < 3; ++a)
	{
		double invD = 1.0f / r.direction()[a];
		double t0 = (min()[a] - r.origin()[a]) * invD;
		double t1 = (max()[a] - r.origin()[a]) * invD;
		if (invD < 0.0f)
			std::swap(t0, t1);
		tmax = t1 < tmax ? t1 : tmax;
		tmin = t0 > tmin ? t0 : tmin;
		if (tmax <= tmin)
			return false;
	}
	return true;
}


//计算2个box的包围盒
aabb surrouding_box(aabb box0, aabb box1)
{
	vec3 small(ffmin(box0.min().x(), box1.min().x()),
			   ffmin(box0.min().y(), box1.min().y()),
			   ffmin(box0.min().z(), box1.min().z()));
	vec3 big(ffmax(box0.max().x(), box1.max().x()),
			 ffmax(box0.max().y(), box1.max().y()),
			 ffmax(box0.max().z(), box1.max().z()));
	return aabb(small, big);

}



#endif // BOUNDING_BOX.H