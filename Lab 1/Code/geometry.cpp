#include "geometry.h"

/* Returns a vector of four vertices representing a face. */
std::vector<vertex> newFace (vertex topLeft, vertex bottomLeft, vertex topRight, vertex bottomRight) {
    std::vector<vertex> f(4);
    f[0] = topLeft; f[1] = bottomLeft; f[2] = bottomRight; f[3] = topRight;
    return f;
}

/* Returns a vector of six vertices representing a quad/face. Vertices are ordered counter-clockwise. */
std::vector<vertex> newQuad (vertex topLeft, vertex bottomLeft, vertex topRight, vertex bottomRight) {
    std::vector<vertex> q(6);
    q[0] = topRight; q[1] = topLeft; q[2] = bottomLeft;         // Triangle 1.
    q[3] = topRight; q[4] = bottomLeft; q[5] = bottomRight;     // Triangle 2.
    return q;
}

/* Returns a vector of thirty-six verticies representing a cube. Verticies ordered counter-clockwise per face. */
std::vector<vertex> newCube (std::vector<vertex> front, std::vector<vertex> back) {
    std::vector<vertex> cube; cube.reserve(36);

    // Sanity Check: Nobody have better given anything without 4 points per face.
    if (front.size() != 4 || back.size() != 4) {
        qDebug() << "Error: A cube takes two faces of four points. Not two quads of six points!";
        exit(EXIT_FAILURE);
    }

    // Create Front, Back, Left, Right, Top, Bottom quads. Insert them consecutively.
    std::vector<vertex> frontQuad   = newQuad(front[0], front[1], front[2], front[3]);  cube.insert(cube.end(), frontQuad.begin(), frontQuad.end());
    std::vector<vertex> backQuad    = newQuad(back[0], back[1], back[2], back[3]);      cube.insert(cube.end(), backQuad.begin(), backQuad.end());
    std::vector<vertex> leftQuad    = newQuad(back[3], back[2], front[1], front[0]);    cube.insert(cube.end(), leftQuad.begin(), leftQuad.end());
    std::vector<vertex> rightQuad   = newQuad(front[3], front[2], back[1], back[0]);    cube.insert(cube.end(), rightQuad.begin(), rightQuad.end());
    std::vector<vertex> topQuad     = newQuad(back[3], front[0], front[3], back[0]);    cube.insert(cube.end(), topQuad.begin(), topQuad.end());
    std::vector<vertex> bottomQuad  = newQuad(front[1], back[2], back[1], front[2]);    cube.insert(cube.end(), bottomQuad.begin(), bottomQuad.end());

    return cube;
}

/* Returns a vector of eightteen vertices representing a pyramid. Vertices are ordered counter-clockwise. */
std::vector<vertex> newPyramid (std::vector<vertex> base, vertex crown) {
    std::vector<vertex> pyramid; pyramid.reserve(18);

    // Sanity Check: Nobody have better given anything without 4 points per face.
    if (base.size() != 4) {
        qDebug() << "Error: A pyramid takes a base of four points. Not " << base.size() << "!";
        exit(EXIT_FAILURE);
    }

    // Create and insert Base. Then just insert Left, Right, Front, Back triangles.
    std::vector<vertex> bottomQuad = newQuad(base[0], base[1], base[2], base[3]); pyramid.insert(pyramid.end(), bottomQuad.begin(), bottomQuad.end());
    pyramid.push_back(crown); pyramid.push_back(base[1]); pyramid.push_back(base[2]);   // Front.
    pyramid.push_back(crown); pyramid.push_back(base[3]); pyramid.push_back(base[0]);   // Back.
    pyramid.push_back(crown); pyramid.push_back(base[0]); pyramid.push_back(base[1]);   // Left.
    pyramid.push_back(crown); pyramid.push_back(base[2]); pyramid.push_back(base[3]);   // Right.

    return pyramid;
}

/* Displays the coordinates within a vector nicely */
void showVector (std::vector<vertex> v) {
    for (unsigned long i = 0; i < v.size(); i++) {
        qDebug() << "" << i << ": (" << v[i].x << "," << v[i].y << v[i].z << ")";
    }
}




