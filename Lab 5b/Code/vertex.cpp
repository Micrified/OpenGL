#include "vertex.h"

/* Constructor: (x, y, z, nx, ny, nz, tx, ty) */
vertex newVertex (float x, float y, float z, float nx, float ny, float nz, float tx, float ty) {
    return {.x = x, .y = y, .z = z, .nx = nx, .ny = ny, .nz = nz, .tx = tx, .ty = ty};
}
