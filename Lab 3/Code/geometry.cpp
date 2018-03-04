#include "geometry.h"

/* Converts a QVector3D object into a vertex vector with a normal component. */
std::vector<vertex> vectorFrom3D(QVector<QVector3D> vertices, QVector<QVector3D> normals, QVector<QVector2D> textureCoordinates) {
    std::vector<vertex> vs;

    // Extract vertex data, and normal data.
    QVector3D *vertexData = vertices.data(), *normalData = normals.data();

    // Extract texture data.
    QVector2D *textureData = textureCoordinates.data();

    vs.reserve(vertices.size());

    for (int i = 0; i < vertices.size(); i++) {

        // 1. Assign vector coordinates (x, y, z).
        float x = vertexData[i].x(), y = vertexData[i].y(), z = vertexData[i].z();

        // 2. Assign normal vectors (nx, ny, nz).
        float nx = normalData[i].x(), ny = normalData[i].y(), nz = normalData[i].z();

        // 3. Assign texture coordinates (tx, ty).
        float tx = textureData[i].x(), ty = textureData[i].y();

        // Push new vertex onto stack.
        vs.push_back(newVertex(x, y, z, nx, ny, nz, tx, ty));
    }

    return vs;
}

/* Displays the coordinates within a vector nicely */
void showVector (std::vector<vertex> v) {
    for (unsigned long i = 0; i < v.size(); i++) {
        qDebug() << "" << i << ": (" << v[i].x << "," << v[i].y << v[i].z << ")";
    }
}




