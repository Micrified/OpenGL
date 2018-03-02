#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the inputs to the fragment shader
// These must have the same type and name!

// IN: The vertex.
in vec3 vertexCoordinate;

// IN: Normal vector from vertex shader.
in vec3 vertexNormal;

// IN: Light position from vertex shader.
in vec3 lightCoordinate;

// IN: Material from vertex shader.
in vec4 material;

// Specify the Uniforms of the fragment shaders
// uniform vec3 lightPosition; // for example

// UNIFORM: Sampler for extracting color from texture.
uniform sampler2D samplerUniform; // TODO: Store uniform location. Delete when done.

// Specify the output of the fragment shader
// Usually a vec4 describing a color (Red, Green, Blue, Alpha/Transparency)
out vec4 fColor;

void main()
{
    // Set light intensities.
    float delta_lv = distance(lightCoordinate, vertexCoordinate);
    float is = 1 / (1 - 0.25 * delta_lv * delta_lv);
    float id = 1 / (1 - 0.25 * delta_lv * delta_lv);
    float ia = 0.1;

    // Set local material constants.
    float ka = material[0];
    float kd = material[1];
    float ks = material[2];
    float n = material[3];

    // Set local light constant.
    vec3 L = normalize(vertexCoordinate - lightCoordinate);
    vec3 N = vertexNormal;

    // Compute Reflection.
    vec3 R = normalize(reflect(L, N));

    // Compute Viewing vector.
    vec3 V = normalize(vec3(0, 0, 0) - vertexCoordinate);

    // Compute Phong.
    float I = ka + (kd * max(dot(L, N),0) + ks * pow(max(dot(R, V),0), n));
    fColor = vec4(I, I, I, 1.0);
}
