#version 330 core

// Define constants
#define M_PI 3.141593

// The number of waves.
#define N_WAVES 8

/*
********************************************************************************
*                                  Attributes                                  *
********************************************************************************
*/

// Vertex Coordinates.
layout (location = 0) in vec3 vertexCoordinate;

// Normal Vector or color. Depends which it is being used for.
layout (location = 1) in vec3 normalVector;

// uv Coordiantes.
layout (location = 2) in vec2 uvCoordinate;

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

// Export: The adjusted vertex.
out vec2 uvCoordinateFrag;


/*
********************************************************************************
*                                    Main                                      *
********************************************************************************
*/

float waveHeight (int waveIdx, float uvalue) {
    float theta = 2 * M_PI * (frequencyUniform[waveIdx] * uvalue + elapsedTimeUniform + phaseUniform[waveIdx]);
    return amplitudeUniform[waveIdx] * sin(theta);
}

float waveDU (int waveIdx, float uvalue) {
    float theta = 2 * M_PI * (frequencyUniform[waveIdx] * uvalue + elapsedTimeUniform + phaseUniform[waveIdx]);
    return 2 * M_PI * amplitudeUniform[waveIdx] * frequencyUniform[waveIdx] * cos(theta);
}


void main () {

    // Copy vertexCoordinate into local variable.
    vec3 currentVertexPosition = vertexCoordinate;

    // Initialize sums to zero.
    currentVertexPosition.z = 0;
    float du = 0;

    // Compute sums.
    for(int i = 0; i < N_WAVES; i++) {
        currentVertexPosition.z += waveHeight(i, currentVertexPosition.x);
        du += waveDU(i, currentVertexPosition.x);
    }

    // Compute adjusted vertex.
    gl_Position = perspectiveUniform * vertexTransformUniform * vec4(currentVertexPosition, 1.0);

    uvCoordinateFrag = uvCoordinate;

}
