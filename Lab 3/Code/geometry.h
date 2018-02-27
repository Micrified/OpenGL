#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QVector3D>
#include <vector>
#include <QOpenGLDebugLogger>
#include <cstdlib>
#include "vertex.h"

/* Converts a QVector3D object into a vertex vector with a normal component. */
std::vector<vertex> vectorFrom3D(QVector<QVector3D> vertices, QVector<QVector3D> normals);

/* Returns a vector of four vertices representing a face. */
std::vector<vertex> newFace (vertex topLeft, vertex bottomLeft, vertex topRight, vertex bottomRight);

/* Returns a vector of six vertices representing a quad/face. Vertices are ordered counter-clockwise. */
std::vector<vertex> newQuad (vertex topLeft, vertex bottomLeft, vertex topRight, vertex bottomRight);

/* Returns a vector of thirty-six verticies representing a cube. Verticies ordered counter-clockwise per face. */
std::vector<vertex> newCube (std::vector<vertex> front, std::vector<vertex> back);

/* Returns a vector of eightteen vertices representing a pyramid. Vertices are ordered counter-clockwise. */
std::vector<vertex> newPyramid (std::vector<vertex> base, vertex crown);

/* Displays the coordinates within a vector nicely */
void showVector (std::vector<vertex> v);

#endif // GEOMETRY_H
