#ifndef CUBEH
#define CUBEH

#include "triangle.h"

class cube: public hitable {
    public:
        cube() {}
        cube(vec3 o, float len, material* mat_ptr): origin(o), length(len), mat_ptr(mat_ptr) {};
        virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
        void rotation(const vec3& rotationAxis, const float theta);

        vec3 origin; // lower-left-front point
        double length;
        material* mat_ptr;

        vec3 lower_left_front = origin;
        vec3 lower_right_front = vec3(origin.x() + length, origin.y(), origin.z());
        vec3 upper_right_front = vec3(origin.x() + length, origin.y() + length, origin.z());
        vec3 upper_left_front = vec3(origin.x(), origin.y() + length, origin.z());

        vec3 lower_left_back = vec3(origin.x(), origin.y(), origin.z() - length);
        vec3 lower_right_back = vec3(origin.x() + length, origin.y(), origin.z() - length);
        vec3 upper_right_back = vec3(origin.x() + length, origin.y() + length, origin.z() - length);
        vec3 upper_left_back = vec3(origin.x(), origin.y() + length, origin.z() - length);

};

void cube::rotation(const vec3& rotationAxis, const float theta) {
    lower_left_front = rotate(lower_left_front, rotationAxis, theta);
    lower_right_front = rotate(lower_right_front, rotationAxis, theta);
    upper_right_front = rotate(upper_right_front, rotationAxis, theta);
    upper_left_front = rotate(upper_left_front, rotationAxis, theta);
    lower_left_back = rotate(lower_left_back, rotationAxis, theta);
    lower_right_back = rotate(lower_right_back, rotationAxis, theta);
    upper_right_back = rotate(upper_right_back, rotationAxis, theta);
    upper_left_back = rotate(upper_left_back, rotationAxis, theta);
}

bool cube::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    // One side has two triangles, so there will be 12 triangles

    // 12 triangles
    triangle front_btm(lower_left_front, lower_right_front, upper_left_front, mat_ptr);
    triangle front_top(lower_right_front, upper_right_front, upper_left_front, mat_ptr);
    
    triangle back_btm(lower_right_back, lower_left_back, upper_right_back, mat_ptr);
    triangle back_top(lower_left_back, upper_left_back, upper_right_back, mat_ptr);

    triangle left_btm(lower_left_back, lower_left_front, upper_left_back, mat_ptr);
    triangle left_top(lower_left_front, upper_left_front, upper_left_back, mat_ptr);

    triangle right_btm(lower_right_front, lower_right_back, upper_right_front, mat_ptr);
    triangle right_top(lower_right_back, upper_right_back, upper_right_front, mat_ptr);

    triangle up_btm(upper_left_front, upper_right_front, upper_left_back, mat_ptr);
    triangle up_top(upper_right_front, upper_right_back, upper_left_back, mat_ptr);

    triangle down_btm(lower_left_back, lower_right_back, lower_left_front, mat_ptr);
    triangle down_top(lower_right_back, lower_right_front, lower_left_front, mat_ptr);

    triangle list[12];
    list[0] = front_btm;
    list[1] = front_top;
    list[2] = back_btm;
    list[3] = back_top;
    list[4] = left_btm;
    list[5] = left_top;
    list[6] = right_btm;
    list[7] = right_top;
    list[8] = up_btm;
    list[9] = up_top;
    list[10] = down_btm;
    list[11] = down_top;

    bool ifHit = false;
    float min_t = FLT_MAX;
    hit_record min_rec;

    for (int i = 0; i < 12; i++) {
        if (list[i].hit(r, t_min, t_max, rec)) {
            ifHit = true;
            if (rec.t < min_t) {
                min_t = rec.t;
                min_rec = rec;
            }
        }
    }

    rec = min_rec;
    return ifHit;
}

#endif