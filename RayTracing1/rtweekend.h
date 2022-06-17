#pragma once
#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include<cmath>
#include<cstdlib>
#include<limits>
#include<memory>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degress_to_radians(double degrees) {
	return degrees * pi / 180.0;
}inline double random_double() {
	//[0,1)
	return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
	//[min,max)
	return min + (max - min) * random_double();
}

inline int random_int(int min, int max)
{
	return (rand() % (max - min)) + min;
}

inline double clamp(double x, double min, double max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}
inline double ffmax(double a, double b)
{
	if (a < b)
		return b;
	else
		return a;
}
inline double ffmin(double a, double b)
{
	if (a > b)
		return b;
	else
		return a;
}
#include"ray.h"
#include"vec3.h"

#endif // !RTWEEKEND_H
