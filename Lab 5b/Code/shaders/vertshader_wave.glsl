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

// Time Uniform: Elapsed time.
uniform float elapsedTimeUniform;

/*
********************************************************************************
*                              Exported Variables                              *
********************************************************************************
*/

// Export: The adjusted vertex.
out vec2 uvCoordinateFrag;

out vec3 normal;


/*
********************************************************************************
*                                    Main                                      *
********************************************************************************
*/

void main () {

    vec3 currentVertexPosition = vertexCoordinate;

    float amplitude = 0.1;
    float time = elapsedTimeUniform;
    float phase = 0;
    float frequency = 10;

    currentVertexPosition.z = amplitude * sin(2 * M_PI * (frequency * uvCoordinate.x + time));

    float du = 2 * M_PI * amplitude * frequency * cos(2 * M_PI * (frequency * uvCoordinate.x + time));

    normal = normalTransformUniform * normalize(vec3(-du, 0, 1.0));

    gl_Position = perspectiveUniform * vertexTransformUniform * vec4(currentVertexPosition, 1.0);

    uvCoordinateFrag = uvCoordinate;
}
