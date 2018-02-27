#ifndef VERTEX_H
#define VERTEX_H

typedef struct vertex {
    float x, y, z;      // Coordinate points.
    float nx, ny, nz;   // Normal vector components.
} vertex;


vertex newVertex (float, float, float, float, float, float);

#endif // VERTEX_H
