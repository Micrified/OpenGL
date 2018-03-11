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

// Returns the color on the sphere for a given hit point, accounting
// for rotation and use of textures.
Color Sphere::getTextureColorAtPoint (Point p){
    static bool initializedRotationInformation = false;
    static Vector unit;
    static Vector rotationAxis;
    static double rotationAngle;

    // Rename point coordinates variables and sphere center more conveniently.
    double x = p.x, y = p.y, z = p.z;
    double cx = position.x, cy = position.y, cz = position.z;

    // Initialization of constants for Rodrigues rotation forumla. 
    if (initializedRotationInformation == false) {
        rotation.normalize();
        unit = Vector(0, 0, 1);
        rotationAxis = rotation.cross(unit);
        rotationAngle = acos(rotation.dot(unit));
        rotationAxis.normalize();
        initializedRotationInformation = true;
    }
    
    // Compute vector from sphere center to hitpoint.
    Vector v(x - cx, y - cy, z - cz);
    
    // If the rotation-vector is identical to the unit vector, then no rotation necessary.
    if(!rotationAxis.isEqual(unit)) {
       v = v * cos(rotationAngle) + (rotationAxis.cross(v)) * sin(rotationAngle) + rotationAxis * (rotationAxis.dot(v)) * (1 - cos(rotationAngle));
    }

    // Calculate texture mapping (rotated coordinate -> polar coordinate -> U,V)
    double theta = acos((-v.z) / r);
    double phi = atan2(v.y, v.x) - (this->angle / 180 * M_PI);

    // Ensure non-negative phi.
    while(phi < 0) {
        phi += 2 * M_PI;
    }

    double u_coord = phi / (2 * M_PI);
    double v_coord = (M_PI - theta) / M_PI;

    return material.texture->colorAt(u_coord, v_coord);
}