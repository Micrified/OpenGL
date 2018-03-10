#include "sphere.h"
#include "solvers.h"

#include <cmath>

using namespace std;

Sphere::Sphere (Point const &pos, double radius, Vector rotation, double angle)
:
    position(pos),
    r(radius),
    rotation(rotation),
    angle(angle)
{}

Hit Sphere::intersect(Ray const &ray)
{
    // Sphere formula: ||x - position||^2 = r^2
    // Line formula:   x = ray.O + t * ray.D

    Vector L = ray.O - position;
    double a = ray.D.dot(ray.D);
    double b = 2 * ray.D.dot(L);
    double c = L.dot(L) - r * r;

    double t0;
    double t1;
    if (not Solvers::quadratic(a, b, c, t0, t1))
        return Hit::NO_HIT();

    // t0 is closest hit
    if (t0 < 0)  // check if it is not behind the camera
    {
        t0 = t1;    // try t1
        if (t0 < 0) // both behind the camera
            return Hit::NO_HIT();
    }

    // calculate normal
    Point hit = ray.at(t0);
    Vector N = (hit - position).normalized();

    // determine orientation of the normal
    if (N.dot(ray.D) > 0)
        N = -N;

    return Hit(t0, N);
}

Color Sphere::getTextureColorAtPoint (Point p){
    double x = p.x, y = p.y, z = p.z;
    double cx = position.x, cy = position.y, cz = position.z;

    double theta = acos((cz - z) / r);
    double phi = atan2(y - cy, x - cx) - this->angle;

    if(phi < 0) {
        phi += 2 * M_PI;
    }

    double u = phi / (2 * M_PI);
    double v = (M_PI - theta) / M_PI;

    return material.texture->colorAt(u, v);
}