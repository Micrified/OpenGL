#include "sphere.h"

#include <cmath>

using namespace std;

Hit Sphere::intersect(Ray const &ray)
{
    
    // place holder for actual intersection calculation
    Vector diff = ray.O - position;
    double radicand = ((ray.D.dot(diff) * ray.D.dot(diff)) - diff.length_2()) + (r * r);

    // Return if a miss.
    if (radicand < 0) {
        return Hit::NO_HIT();
    }

    // Compute distance.
    double d = -(ray.D.dot(diff)) - sqrt(radicand);
    Point intPoint = ray.at(d);


    Vector N = Triple(intPoint.x - position.x, intPoint.y - position.y, intPoint.z - position.z);
    N.normalize();

    return Hit(d,N);
}

Sphere::Sphere(Point const &pos, double radius)
:
    position(pos),
    r(radius)
{}
