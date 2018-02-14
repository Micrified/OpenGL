#include "geometry.h"


/* Returns a new quad given the corners. */
vertex *setQuad (vertex *quad, vertex topLeft, vertex bottomLeft, vertex topRight, vertex bottomRight) {
    quad[0] = topRight;
    quad[1] = topLeft;
    quad[2] = bottomLeft;
    quad[3] = topRight;
    quad[4] = bottomLeft;
    quad[5] = bottomRight;

    return quad;
}

vertex *setTri (vertex *pp, vertex tip, vertex left, vertex right) {
    pp[0] = tip;
    pp[1] = left;
    pp[2] = right;
}

/* Returns a cube given two faces (a = points of face 1, b = points of face 2) */
void setCube (vertex *cp, vertex a1, vertex a2, vertex a3, vertex a4, vertex b1, vertex b2, vertex b3, vertex b4) {

    // Write Given faces.
    setQuad(cp, a1, a2, a3, a4);
    setQuad(cp + 6, b1, b2, b3, b4);

    // Write Deduced faces.
    setQuad(cp + 12, b2, a4, b4, a2);
    setQuad(cp + 18, a3, a4, b1, b2);
    setQuad(cp + 24, a3, b1, a1, b3);
    setQuad(cp + 30, b3, b4, a1, a2);
}

void setPyramid (vertex *pp, vertex a1, vertex a2, vertex a3, vertex a4, vertex tip) {

    setQuad(pp, a1, a2, a3, a4);
    setTri(pp + 6, tip, a2, a1);
    setTri(pp + 9, tip, a4, a2);
    setTri(pp + 12, tip, a3, a4);
    setTri(pp + 15, tip, a1, a3);
}



