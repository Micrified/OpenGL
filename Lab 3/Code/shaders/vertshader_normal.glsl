#version 330 core

// Define constants
#define M_PI 3.141593


/*
 ****************************************************
 *                   Attributes                     *
 ****************************************************
*/

// The vertex coordinate (unmodified).
layout (location = 0) in vec3 vertCoordinates_in;

// The normal vector (unmodified).
layout (location = 1) in vec3 vertNormal_in;

// The texture coordinate.
layout (location = 2) in vec2 textCoordinates_in;

/*
 ****************************************************
 *                    Uniforms                      *
 ****************************************************
*/

// UNIFORM: Model Transform = translation + rotation + scaling.
uniform mat4 modelTransformUniform;

// UNIFORM: Perspective.
uniform mat4 perspectiveUniform;

// UNIFORM: Normal transform for all vertices.
uniform mat3 normalTransformUniform;

// UNIFORM: The light-coordinate uniform.
uniform vec3 lightCoordinateUniform;

// UNIFORM: The surface material.
uniform vec4 materialUniform;

/*
 ****************************************************
 *                 Outgoing Data                    *
 ****************************************************
*/

// OUT: Specify the output of the vertex stage
out vec3 vertexNormal;

void main()
{
    // gl_Position is the output (a vec4) of the vertex shader
    gl_Position = perspectiveUniform * modelTransformUniform * vec4(vertCoordinates_in, 1.0);

    // Export the adjusted normal vector.
    vertexNormal = normalTransformUniform * vertNormal_in;

}
