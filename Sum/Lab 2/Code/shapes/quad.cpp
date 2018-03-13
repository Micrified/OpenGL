#include "quad.h"

#include <cmath>

#define THRESHHOLD      1E-5

using namespace std;


Hit Quad::intersect(Ray const &ray)
{
    Triangle *tri1 = new Triangle(a, b, c);
    Triangle *tri2 = new Triangle(c, b, d);
    Hit h1 = tri1->intersect(ray);
    Hit h2 = tri2->intersect(ray);


    if (isnan(h1.t)){ return h2;}
    if (isnan(h2.t)){ return h1;}
    return h1.t < h2.t ? h1 : h2;
}

Quad::Quad(Point a, Point b, Point c, Point d)
:
    a(a), 
    b(b), 
    c(c),
    d(d)
{}
