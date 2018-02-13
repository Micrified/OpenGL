#include "vertex.h"

vertex newVertex (float x, float y, float z, float r, float g, float b) {
    return {.x = x, .y = y, .z = z, .r = r, .g = g, .b = b};
}
