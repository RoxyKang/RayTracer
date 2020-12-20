cmake_minimum_required ( VERSION 3.1.0 )

project (RayTracer
             VERSION 3.0.0
             LANGUAGES CXX
)

#Set to c++11
set ( CMAKE_CXX_STANDARD 11 )

#Sources

set (SOURCE_ONE_WEEKEND
       src/main.cc
       src/vec3.h
       src/ray.h
       src/hitable.h
       src/sphere.h
       src/hitable_list.h
       src/camera.h
       src/material.h
       src/cube.h
       src/triangle.h
       src/light.h
       src/cone.h
)

#Executables
add_executable(inOneWeekend ${SOURCE_ONE_WEEKEND})