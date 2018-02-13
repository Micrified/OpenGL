#ifndef VERTEX_H
#define VERTEX_H

typedef struct vertex {
    float x, y, z;      // Coordinate points.
    float r, g, b;      // Color channels.
} vertex;


vertex newVertex (float, float, float, float, float, float);

#endif // VERTEX_H
