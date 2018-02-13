#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "vertex.h"

/* Returns a new quad given the corners. */
vertex *setQuad (vertex *quad, vertex topLeft, vertex bottomLeft, vertex topRight, vertex bottomRight);


/* Returns a cube given two faces (a = points of face 1, b = points of face 2) */
void setCube (vertex *cp, vertex a1, vertex a2, vertex a3, vertex a4, vertex b1, vertex b2, vertex b3, vertex b4);

#endif // GEOMETRY_H
