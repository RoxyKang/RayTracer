#ifndef MATERIALH
#define MATERIALH

#include "hitable.h"
#include <algorithm>
#include "light.h"

class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;

	virtual vec3 blinnShdingCalc(const ray& r_in, vec3& lightVec, const hit_record& rec, light* l) {
		vec3 color;
		vec3 view = unit_vector(r_in.direction());
		lightVec = unit_vector(lightVec);
		vec3 lightColor = l->getColor();

		// Diffuse component
		float NLAngle = std::max(0.0, dot(lightVec, rec.normal));
		vec3 diffuse = lightColor * NLAngle;

		// Specular component

		vec3 reflectionVec = reflectBlinn(view, lightVec);
		float specAmount = pow(std::max(0.0, dot(reflectionVec, rec.normal)), shininess);
		vec3 specular = specAmount * lightColor;
		color = kAmbient * ambient + kDiffuse * diffuse + kSpecular * specular;

		return color;
	}

	vec3 ambient = vec3(dis(gen), dis(gen), dis(gen));
	float shininess = 10.0 * dis(gen);
	float kAmbient = dis(gen);
	float kDiffuse = dis(gen);
	float kSpecular = dis(gen);
};

class lambertian : public material {
public:
	lambertian(const vec3& a) : albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}

	vec3 albedo;
};

class metal : public material {
public:
	metal(const vec3& a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }


	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

	vec3 albedo;
	float fuzz;
};

class dielectric : public material {
public:
	dielectric(float ri) : ref_idx(ri) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
		vec3 outward_normal;
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		float ni_over_nt;
		attenuation = vec3(1.0, 1.0, 1.0);
		vec3 refracted;
		float reflect_prob;
		float cosine;
		if (dot(r_in.direction(), rec.normal) > 0) {
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}
		else { // inner to outer
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
			cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}

		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
			reflect_prob = schlick(cosine, ref_idx);
		}
		else {
			scattered = ray(rec.p, reflected);
			reflect_prob = 1.0;
		}

		if (dis(gen) < reflect_prob) {
			scattered = ray(rec.p, reflected);
		}
		else {
			scattered = ray(rec.p, refracted);
		}
		return true;
	}

	float ref_idx;
};

#endif