#include "triangle.h"

#include <cmath>

#define THRESHHOLD      1E-5

using namespace std;

bool Triangle::leftOf (Vector p, Vector e, Vector N) {
    Vector x = e.cross(p);
    return (N.dot(x) >= 0);
}

bool Triangle::withinTriangle (Point P, Vector N) {
    return leftOf(P - a, b - a, N) && 
           leftOf(P - b, c - b, N) && 
           leftOf(P - c, a - c, N);
}

Hit Triangle::intersect(Ray const &ray)
{

    //cerr << "Triangle: Intersect ";

    // Calculating the triangle surface normal.
    Vector U = b - a, V = c - a;
    Vector N = U.cross(V);
    N.normalize();

    // **** Calculate the intersection ****
    // 1. Find (triangle face normal dot direction of ray).
    double denominator = N.dot(ray.D);

    // 2. If miss (parallel or otherwise), return no hit.
    if (fabs(denominator) < THRESHHOLD) {
        return Hit::NO_HIT();
    }

    //cerr << "Triangle: denominator > threshhold ";

    // 3. Compute intersection point.
    double d = N.dot(a);
    double t = (d - N.dot(ray.O)) / denominator;

    // 4. If intersection point is less than zero, we didn't intersect.
    if (t < 0) {
        return Hit::NO_HIT();
    }

    //cerr << "Triangle: t >= 0";

    // 5. Ensure point lies within triangle.
    if (!withinTriangle(ray.O + t * ray.D, N)) {
        return Hit::NO_HIT();
    }

    //cerr << "Triangle: Hit";

    return Hit(t, N);
}

Triangle::Triangle(Point a, Point b, Point c)
:
    a(a), 
    b(b), 
    c(c)
{}
