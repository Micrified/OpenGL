#ifndef QUAD_H_
#define QUAD_H_

#include "../object.h"
#include "triangle.h"
#include <exception>
#include <fstream>
#include <iostream>

class Quad: public Object
{
    public:
        Quad(Point a, Point b, Point c, Point d);


        virtual Hit intersect(Ray const &ray);

        Point a, b, c, d;
};

#endif
