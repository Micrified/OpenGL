#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "../object.h"
#include <exception>
#include <fstream>
#include <iostream>

class Triangle: public Object
{
    public:
        Triangle(Point a, Point b, Point c);

        bool leftOf (Vector p, Vector e, Vector N);
        bool withinTriangle (Point P, Vector N);

        virtual Hit intersect(Ray const &ray);

        Point a, b, c;
};

#endif
