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

// OUT: Vertex.
out vec3 vertexCoordinate;

// OUT: Normal Vector.
out vec3 vertexNormal;

// OUT: Light position.
out vec3 lightCoordinate;

// OUT: Material.
out vec4 material;


void main()
{
    // gl_Position is the output (a vec4) of the vertex shader
    gl_Position = perspectiveUniform * modelTransformUniform * vec4(vertCoordinates_in, 1.0);

    // Pass the vertex to to the fragment shader.
    vertexCoordinate = vec3(gl_Position);

    // Compute vertex normal vector.
    vertexNormal = normalTransformUniform * vertNormal_in;

    // Pass light location to fragment shader.
    lightCoordinate = vec4(modelTransformUniform * vec4(lightCoordinateUniform,1)).xyz;

    // Pass material to fragment shader.
    material = materialUniform;
}
