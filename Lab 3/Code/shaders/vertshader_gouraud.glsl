#version 330 core

// Define constants
#define M_PI 3.141593

/*
********************************************************************************
*                                  Attributes                                  *
********************************************************************************
*/

// Vertex Coordinates.
layout (location = 0) in vec3 vertexCoordinate;

// Normal Vector or color. Depends which it is being used for.
layout (location = 1) in vec3 normalVector;

// Texture Coordiantes.
layout (location = 2) in vec2 textureCoordinate;

/*
********************************************************************************
*                                   Uniforms                                   *
********************************************************************************
*/

// Vertex Transformation Uniform: Translation + Rotation + Scaling.
uniform mat4 vertexTransformUniform;

// Normal Transform Uniform: Transformation of normal vector.
uniform mat3 normalTransformUniform;

// Perspective Uniform: Viewing Perspective.
uniform mat4 perspectiveUniform;

// Lighting Uniform: Light Source Coordinate.
uniform vec3 lightCoordinateUniform;

// Material Uniform: Material Properties.
uniform vec4 materialUniform;

/*
********************************************************************************
*                              Exported Variables                              *
********************************************************************************
*/

// Export: Computed light intensity.
out float i;

// Export: The texture coordinate.
out vec2 fragmentTextureCoordinate;

/*
********************************************************************************
*                                    Main                                      *
********************************************************************************
*/

void main () {

    // ******* Computing Required Components ******

    // Compute the adjusted vertex.
    gl_Position = perspectiveUniform * vertexTransformUniform * vec4(vertexCoordinate, 1.0);
    vec3 adjustedVertex = vec4(vertexTransformUniform * vec4(vertexCoordinate, 1.0)).xyz;

    // Compute adjusted light location.
    vec3 adjustedLightCoordinate = vec4(vertexTransformUniform * vec4(lightCoordinateUniform, 1.0)).xyz;

    // ******* Computing Gouraud Shading ******

    // Extract material properties.
    float ka = materialUniform[0], kd = materialUniform[1], ks = materialUniform[2], n = materialUniform[3];

    // Compute adjusted normal vector.
    vec3 N = normalize(normalTransformUniform * normalVector);
    vec3 L = normalize(adjustedLightCoordinate - adjustedVertex);
    vec3 V = normalize(adjustedVertex - vec3(0.0, 0.0, 0.0));
    vec3 R = normalize(reflect(L,N));

    // Ambient.
    float ambient = ka;

    // Diffuse.
    float diffuse = max(dot(N, L), 0) * kd;

    // Specular.
    float specular = pow(max(dot(R, V), 0), n) * ks;

    // Total illumination.
    i = ambient + diffuse + specular;

    // Copy over texture coordinate.
    fragmentTextureCoordinate = textureCoordinate;

}
