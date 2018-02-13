#include "geometry.h"


/* Returns a new quad given the corners. */
vertex *setQuad (vertex *quad, vertex topLeft, vertex bottomLeft, vertex topRight, vertex bottomRight) {
    quad[0] = topLeft;
    quad[1] = bottomLeft;
    quad[2] = topRight;
    quad[3] = topRight;
    quad[4] = bottomLeft;
    quad[5] = bottomRight;

    return quad;
}

/* Returns a cube given two faces (a = points of face 1, b = points of face 2) */
void setCube (vertex *cp, vertex a1, vertex a2, vertex a3, vertex a4, vertex b1, vertex b2, vertex b3, vertex b4) {

    // Write Given faces.
    setQuad(cp, a1, a2, a3, a4);
    setQuad(cp + 6, b1, b2, b3, b4);

    // Write Deduced faces.
    setQuad(cp + 12, a1, a2, b1, b2);
    setQuad(cp + 18, a1, a3, b1, b3);
    setQuad(cp + 24, a2, a4, b2, b4);
    setQuad(cp + 30, a3, a4, b3, b4);
}
