#ifndef PLANE_H_
#define PLANE_H_

#include "../object.h"
#include <exception>
#include <fstream>
#include <iostream>

class Plane: public Object
{
    public:
        Plane(Point a, Vector n);

        virtual Hit intersect(Ray const &ray);

        Point a;
        Vector n;
};

#endif
