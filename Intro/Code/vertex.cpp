#include "vertex.h"

// Initialize a new vertex.
struct vertex newVertex (float x, float y, float r, float g, float b) {
    return {.x = x, .y = y, .r = r, .g = g, .b = b};
}

