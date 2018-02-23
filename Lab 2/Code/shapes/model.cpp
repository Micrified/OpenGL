#include "model.h"

#include <cmath>

#define THRESHHOLD      1E-5

using namespace std;


Hit Model::intersect(Ray const &ray)
{
    Vector N = n.normalized();

    // **** Calculate the intersection ****
    // 1. Find (triangle face normal dot direction of ray).
    double denominator = N.dot(ray.D);

    // 2. If miss (parallel or otherwise), return no hit.
    if (fabs(denominator) < THRESHHOLD) {
        return Hit::NO_HIT();
    }

    // 3. Compute intersection point.
    double d = N.dot(a);
    double t = (d - N.dot(ray.O)) / denominator;

    // 4. If intersection point is less than zero, we didn't intersect.
    if (t < 0) {
        return Hit::NO_HIT();
    }

    return Hit(t, N);
}

Model::Model(std::string a)
:
    mod(object(a));
{}
