#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QVector3D>
#include <QVector2D>
#include <vector>
#include <QOpenGLDebugLogger>
#include <cstdlib>
#include "vertex.h"

/* Converts a QVector3D object into a vertex vector with a normal component. */
std::vector<vertex> vectorFrom3D(QVector<QVector3D> vertices, QVector<QVector3D> normals, QVector<QVector2D> textureCoordinates);

/* Displays the coordinates within a vector nicely */
void showVector (std::vector<vertex> v);

#endif // GEOMETRY_H
