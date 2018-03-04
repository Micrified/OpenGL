#version 330 core

// Define constants
#define M_PI 3.141593

/*
********************************************************************************
*                                   Uniforms                                   *
********************************************************************************
*/

// Sampler Uniform:
uniform sampler2D samplerUniform;

/*
********************************************************************************
*                              Imported Variables                              *
********************************************************************************
*/

// Import: The adjusted vertex.
in vec3 adjustedVertex;

// Import: The adjusted light location.
in vec3 adjustedLightCoordinate;

// Import: The adjusted normal vector.
in vec3 adjustedNormalVector;

// Import: The material.
in vec4 material;

// Import: The texture coordinate.
in vec2 fragmentTextureCoordinate;

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
    float ka = material[0], kd = material[1], ks = material[2], n = material[3];

    // Assign N: The adjusted normal.
    vec3 N = adjustedNormalVector;

    // Compute L: Vector from point to light coordinate.
    vec3 L = normalize(adjustedLightCoordinate - adjustedVertex);

    // Compute V: Our viewing vector (set to 0,0,0).
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

    // Get texture color.
    vec4 textureColor = texture2D(samplerUniform, fragmentTextureCoordinate);

    // Apply Phong Shading.
    fColor = textureColor * i;
}
