#version 330 core

// Define constants
#define M_PI 3.141593


/*
********************************************************************************
*                              Imported Variables                              *
********************************************************************************
*/


// Import: The (u,v) coordinate.
in vec2 uvCoordinateFrag;

// Import: The adjusted vertex.
in vec3 adjustedVertex;

// Import: The adjusted light location.
in vec3 adjustedLightCoordinate;

// Import: The adjusted normal vector.
in vec3 adjustedNormalVector;

// Import: The material.
in vec4 material;

in float height;

/*
********************************************************************************
*                              Exported Variables                              *
********************************************************************************
*/

// Export: Fragment shader color (RGBA).
out vec4 fColor;

/*
********************************************************************************
*                                    Main                                      *
********************************************************************************
*/

void main ()
{
    // Extract material components.
    float ka = 0.2, kd = 0.8, ks = 0.8, n = 64;

    // Assign N: The adjusted normal.
    vec3 N = normalize(adjustedNormalVector);

    // Compute L: Vector from point to light coordinate.
    vec3 L = normalize(adjustedLightCoordinate - adjustedVertex);

    // Compute V: Our viewing angle (set to 0,0,0).
    vec3 V = normalize(adjustedVertex - vec3(0.0, 0.0, 0.0));

    // Compute R: The reflected L vector using our adjusted normal.
    vec3 R = normalize(reflect(L,N));

    // Ambient.
    float ambient = ka;

    // Diffuse.
    float diffuse = max(dot(N, L), 0) * kd;

    // Specular.
    float specular = pow(max(dot(R, V), 0), n) * ks;

    // Total illumination.
    float i = ambient + diffuse + specular;

    // Somehow blend in wave color here.

    float s = smoothstep(-0.135, 0.16, height);

    vec3 colour = mix(vec3(0,0,1), vec3(1,1,1), s);

    // Apply uv Shading.
    fColor = vec4(colour, 1.0) * i;
}
