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

// Time Uniform: Elapsed time.
uniform float elapsedTimeUniform;

/*
********************************************************************************
*                              Exported Variables                              *
********************************************************************************
*/

// Export: The adjusted vertex.
out vec3 adjustedVertex;

// Export: The adjusted light location.
out vec3 adjustedLightCoordinate;

// Export: The adjusted normal vector.
out vec3 adjustedNormalVector;

// Export: The material.
out vec4 material;

// Export: The texture coordinate.
out vec2 fragmentTextureCoordinate;

/*
********************************************************************************
*                                    Main                                      *
********************************************************************************
*/

void main () {

    // Compute the adjusted vertex.
    gl_Position = perspectiveUniform * vertexTransformUniform * vec4(vertexCoordinate, 1.0);
    adjustedVertex = vec4(vertexTransformUniform * vec4(vertexCoordinate, 1.0)).xyz;

    // Compute adjusted light location.
    adjustedLightCoordinate = vec4(vertexTransformUniform * vec4(lightCoordinateUniform, 1.0)).xyz;

    // Compute adjusted normal vector.
    adjustedNormalVector = normalize(normalTransformUniform * normalVector);

    // Copy over material.
    material = materialUniform;

    // Copy over texture coordinate.
    fragmentTextureCoordinate = textureCoordinate;
}
