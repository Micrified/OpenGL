#ifndef VERTEX_CPP
#define VERTEX_CPP
#include <stdlib.h>
#include <vector>

//******************************************************************************
//                           Vertex Struct Definition
//******************************************************************************

struct vertex {
    float x,y;      // Coordinates.
    float r,g,b;    // RGB Channel Components.
};

//******************************************************************************
//                                 Functions
//******************************************************************************

// Initialize a new vertex.
struct vertex newVertex (float x, float y, float r, float g, float b);





#endif // VERTEX_CPP
