#include <iostream>
#include <random>
#include "sphere.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"
#include "light.h"
#include "cube.h"
#include "cone.h"

hitable* random_scene() {
	int n = 500;
	hitable** list = new hitable * [n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = dis(gen);
			vec3 center(a + 0.9 * dis(gen), 0.2, b + 0.9 * dis(gen));
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.8) {
					list[i++] = new sphere(center, 0.2, new lambertian(vec3(dis(gen) * dis(gen), dis(gen) * dis(gen), dis(gen) * dis(gen))));
				}
				else if (choose_mat < 0.95) {
					list[i++] = new sphere(center, 0.2, new metal(vec3(0.5 * (1 + dis(gen)), 0.5 * (1 + dis(gen)), 0.5 * (1 + dis(gen))), 0.5*dis(gen)));
				}
				else {
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new hitable_list(list, i);
}

vec3 color(const ray& r, hitable* world, int depth, light* l) {
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		vec3 lightVec = l->getDirToLight(rec.p);
		vec3 lightPos = l->getPos() + random_in_unit_sphere() * 6.0;

		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			// Soft shadows, +- 2 light pos
			vec3 shadowSum(0, 0, 0);
			vec3 blinnSum(0, 0, 0);
			for (int i = -10; i < 11; i++) {
				light* softPL = new pointLight(vec3(lightPos.x()+0.1*i, lightPos.y(), lightPos.z()), vec3(1.0, 1.0, 1.0));

				vec3 softLightVec = softPL->getDirToLight(rec.p);
				vec3 softLightPos = softPL->getPos() + random_in_unit_sphere() * 6.0;

				ray shadowRay = ray(rec.p, softLightPos - rec.p);

				if (world->hit(shadowRay, 0.001, FLT_MAX, rec)) {
					shadowSum += vec3(0.1, 0.1, 0.1);
				}
				else {
					shadowSum += attenuation;
				}

				blinnSum += rec.mat_ptr->blinnShdingCalc(shadowRay, softLightVec, rec, softPL);
			}

			vec3 contribution = shadowSum / 21.0;
			vec3 c = rec.mat_ptr->blinnShdingCalc(ray(rec.p, lightPos - rec.p), lightVec, rec, l);

			return contribution * color(scattered, world, depth + 1, l) + contribution * c;

		}
		else {
			return vec3(0, 0, 0);
		}
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		double t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

int main() {
	int nx = 1200;
	int ny = static_cast <int> (nx/(16.0/9.0));
	int ns = 10;
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";

//	hitable* world = random_scene();
	hitable* list[3];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	cube* c = new cube(vec3(0, 0, 0), 1, new metal(vec3(0.8, 0.3, 0.3), 0.0));
	c->rotation(vec3(0, 0, 1), 45);
//	list[0] = new cone(vec3(0, -1, 0), 30, vec3(0, 1, 0), 15, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[1] = c;
//	list[1] = new sphere(vec3(0, 1, 0), 1, new blinn(vec3(0.4, 0.4, 0.4), 10.0, 0.4, 0.8, 0.8));
//	list[1] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.0));
	list[2] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	hitable* world = new hitable_list(list, 3);

	vec3 lookfrom(13, 2, 13);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.1;
	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, 16.0/9.0, aperture, dist_to_focus);

	light* pl = new pointLight(vec3(1, 5, -15), vec3(1.0, 1.0, 1.0));

	light* dirLight = new directionLight(vec3(0, -1, 0), vec3(1.0, 1.0, 1.0));

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) {
				float u = float(i + dis(gen)) / float(nx);
				float v = float(j + dis(gen)) / float(ny);
				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.0);
				col += color(r, world, 0, pl);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}