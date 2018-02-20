#include "sphere.h"

#include <cmath>

using namespace std;

Hit Sphere::intersect(Ray const &ray)
{
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
    *
    * Given: ray, position, r
    * Sought: intersects? if true: *t
    *
    * Insert calculation of ray/sphere intersection here.
    *
    * You have the sphere's center (C) and radius (r) as well as
    * the ray's origin (ray.O) and direction (ray.D).
    *
    * If the ray does not intersect the sphere, return false.
    * Otherwise, return true and place the distance of the
    * intersection point from the ray origin in *t (see example).
    ****************************************************/

    // Create the unit vector (as a copy) + (assumes D is normalized)
    //Vector l = ray.D;
    
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

    /*Vector OC = (position - ray.O).normalized();
    if (OC.dot(ray.D) < 0.999) {
        return Hit::NO_HIT();
    }
    double t = 1000;*/

    /****************************************************
    * RT1.2: NORMAL CALCULATION
    *
    * Given: t, C, r
    * Sought: N
    *
    * Insert calculation of the sphere's normal at the intersection point.
    ****************************************************/

    Vector N = Triple(position.x - intPoint.x, position.y - intPoint.y, position.z - intPoint.z);
    N.normalize();

    return Hit(d,N);
}

Sphere::Sphere(Point const &pos, double radius)
:
    position(pos),
    r(radius)
{}
