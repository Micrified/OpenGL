#ifndef SPHERE_H_
#define SPHERE_H_

#include "../object.h"

class Sphere: public Object
{
    public:

        Sphere (Point const &pos, double radius, Vector rotation, double angle);

        virtual Hit intersect(Ray const &ray);

        virtual Color getTextureColorAtPoint (Point p);

        Point const position;
        double const r;
        Vector rotation;
        double angle;
};

#endif
