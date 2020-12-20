#ifndef LIGHTH
#define LIGHTH

#include "ray.h"

class light {
public:
	virtual vec3 getColor() {
		return vec3(0, 0, 0);
	};

	virtual vec3 getPos() {
		return vec3(0, 0, 0);
	}

	virtual vec3 getDir() {
		return vec3(0, 0, 0);
	}

	virtual vec3 getDirToLight(vec3& p) {
		return vec3(0, 0, 0);
	}
};

class pointLight : public light {
public:
	pointLight (const vec3& p, const vec3& c) : pos(p), color(c) {}

	virtual vec3 getColor() {
		return color;
	}

	virtual vec3 getPos() {
		return pos;
	}

	virtual vec3 getDirToLight(vec3& p) {
		return pos - p;
	}

	vec3 pos;
	vec3 color;
};

class directionLight : public light {
public:
	directionLight (const vec3& d, const vec3& c) : direction(d), color(c) {}

	virtual vec3 getColor() {
		return color;
	}

	virtual vec3 getDir() {
		return direction;
	}

	virtual vec3 getDirToLight(vec3& p) {
		return -direction;
	}

	vec3 direction;
	vec3 color;
};


#endif
