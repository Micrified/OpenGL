#ifndef SPHERE_H_
#define SPHERE_H_

#include "../object.h"
#include <exception>
#include <fstream>
#include <iostream>

class Sphere: public Object
{
    public:

        Sphere (Point const &pos, double radius, Vector rotation, double angle);

        virtual Hit intersect(Ray const &ray);

        // Returns the color on the sphere for a given hit point, accounting
        // for rotation and use of textures.
        virtual Color getTextureColorAtPoint (Point p);

        // The position of the sphere.
        Point const position;

        // The sphere's radius.
        double const r;

        // The rotation vector.
        Vector rotation;

        // The rotation angle.
        double angle;
};

#endif
