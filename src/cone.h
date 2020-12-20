#ifndef CONEH
#define CONEH

#include "hitable.h"

// Math reference: http://lousodrome.net/blog/light/2017/01/03/intersection-of-a-ray-and-a-cone/
class cone : public hitable {
public:
	cone() {}
	cone(vec3 t, float theta, vec3 v, float h, material* mat_ptr) : tip(t), halfAngle(theta), downVec(v), height(h), mat_ptr(mat_ptr) {};
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;

	vec3 tip;
	float halfAngle;
	vec3 downVec;
	float height;

	material* mat_ptr;
};

bool cone::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	vec3 rayDir = unit_vector(r.direction());
	vec3 rayOrig = r.origin();
	vec3 downVecUnit = unit_vector(downVec);
	vec3 tipOrig = rayOrig - tip;

	float costheta = cos(0.95); // in radian
	float costhetaPow2 = costheta * costheta;

	float a = dot(rayDir, downVecUnit) * dot(rayDir, downVecUnit) - costhetaPow2;
	float b = 2.0 * (dot(rayDir, downVecUnit) * dot(tipOrig, downVecUnit) - dot(rayDir, tipOrig) * costhetaPow2);
	float c = dot(tipOrig, downVecUnit) * dot(tipOrig, downVecUnit) - dot(tipOrig, tipOrig) * costhetaPow2;

	float discriminant = b * b - 4 * a * c;

	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant)) / (2 * a);
		vec3 tipIntersec = r.point_at_parameter(temp) - tip;
		float projectHeight = dot(tipIntersec, downVecUnit);

		if (temp < t_max && temp > t_min && projectHeight >= 0 && projectHeight <= height) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = unit_vector(tipIntersec * dot(downVecUnit, tipIntersec) / dot(tipIntersec, tipIntersec) - downVecUnit);
			rec.mat_ptr = mat_ptr;
			return true;
		}

		temp = (-b + sqrt(discriminant)) / (2 * a);
		tipIntersec = r.point_at_parameter(temp) - tip;
		projectHeight = dot(tipIntersec, downVecUnit);
		if (temp < t_max && temp > t_min && projectHeight >= 0 && projectHeight <= height) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = unit_vector(tipIntersec * dot(downVecUnit, tipIntersec) / dot(tipIntersec, tipIntersec) - downVecUnit);
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}
#endif