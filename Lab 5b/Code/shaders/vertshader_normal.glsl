#version 330 core

// Define constants
#define M_PI 3.141593

// The number of waves.
#define N_WAVES 3

/*
********************************************************************************
*                                  Attributes                                  *
********************************************************************************
*/

// Vertex Coordinates.
layout (location = 0) in vec3 vertexCoordinate;

// Normal Vector or color. Depends which // The number of waves.
#define N_WAVES 3it is being used for.
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

// Amplitude of the waves.
uniform float amplitudeUniform[N_WAVES];

// Frequency of the waves.
uniform float frequencyUniform[N_WAVES];

// Phase of the waves.
uniform float phaseUniform[N_WAVES];

// Time Uniform: Elapsed time.
uniform float elapsedTimeUniform;

/*
********************************************************************************
*                              Exported Variables                              *
********************************************************************************
*/

// Export: Color to be rendered in fragment shader.
out vec3 vertexColor;

/*
********************************************************************************
*                                    Main                                      *
********************************************************************************
*/

void main () {

    // Compute vertex position.
    gl_Position = perspectiveUniform * vertexTransformUniform * vec4(vertexCoordinate, 1.0);

    // Compute the transformed normal vector as the color.
    vertexColor = normalize(normalTransformUniform * normalVector);

}
