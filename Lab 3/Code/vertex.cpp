#include "vertex.h"

vertex newVertex (float x, float y, float z, float nx, float ny, float nz) {
    return {.x = x, .y = y, .z = z, .nx = nx, .ny = ny, .nz = nz};
}
