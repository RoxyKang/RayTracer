#ifndef TRIANGLEH
#define TRIANGLEH

#include "hitable.h"

class triangle : public hitable {
public:
	triangle() {}
	triangle(vec3 vertex0, vec3 vertex1, vec3 vertex2, material* mat_ptr) : a(vertex0), b(vertex1), c(vertex2), mat_ptr(mat_ptr) {};
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;

	vec3 a;
	vec3 b;
	vec3 c;
	material* mat_ptr;
};

bool triangle::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	vec3 ab = b - a;
	vec3 ac = c - a;

	vec3 intersection = rayPlaneIntersection(r.origin(), r.direction(), ab, ac, a);

	if (barycentricCoord(intersection, a, b, c)) {
		vec3 normal = getNormal(ab, ac);
		float temp = dot(normal, (a - r.origin())) / dot(normal, r.direction());
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = intersection;
			rec.normal = normal;
			rec.mat_ptr = mat_ptr;
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

#endif