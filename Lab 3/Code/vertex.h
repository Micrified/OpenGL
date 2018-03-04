#ifndef VERTEX_H
#define VERTEX_H

typedef struct vertex {
    float x, y, z;      // Vertex coordinate points.
    float nx, ny, nz;   // Normal vector components.
    float tx, ty;       // Texture coordiante points.
} vertex;

/* Constructor: (x, y, z, nx, ny, nz, tx, ty) */
vertex newVertex (float x, float y, float z, float nx, float ny, float nz, float tx, float ty);

#endif // VERTEX_H
