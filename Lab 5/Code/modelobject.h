#ifndef MODELOBJECT_H
#define MODELOBJECT_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <memory>

class ModelObject
{

public:
    ModelObject();

    /*
    ********************************************************************************
    *                                 Object Data                                  *
    ********************************************************************************
    */

    // Mesh vertex count.
    GLuint meshVertexCount;

    // Mesh VBO and VAO.
    GLuint mesh_vbo, mesh_vao;

    // Mesh Material: (ka, kd, ks, n).
    std::vector<float> materialVector;

    // Texture pointer.
    GLuint texturePointer;

    // Transform function pointer.
    QMatrix4x4 (*transform)(float time, void *modelObject);

    /*
    ********************************************************************************
    *                              Object Transform                                *
    ********************************************************************************
    */

    // Function for setting object transforms.
    void setTransform(QMatrix4x4 translationMatrix, QMatrix4x4 rotationMatix, QMatrix4x4 scaleMatrix);

    // Matrix representing the current translation.
    QMatrix4x4 translationMatrix;

    // Matrix representing the current rotation.
    QMatrix4x4 rotationMatrix;

    // Matrix representing the current scale.
    QMatrix4x4 scaleMatrix;

    // Function to get the transform matrix at a given time
    QMatrix4x4 transformMatrix(float t);


    /*
    ********************************************************************************
    *                              Object Animation                                *
    ********************************************************************************
    */

    // Function for setting animation settings.
    void setAnimationFactors (float rotx, float roty, float rotz, float transx, float transy, float transz);

    // The rotation factor.
    float rotation[3] = {5.0, 5.0, 0};

    // The translation factor.
    float translation[3] = {1.1, 1.0, 1.0};

    float radius;
};

// Transformation Function: Earth
QMatrix4x4 transform_Earth(float time, void *modelObject);

// Transformation Function: Moon
QMatrix4x4 transform_Moon(float time, void *modelObject);

// Transformation Function: Compound Orbit
QMatrix4x4 transform_CompoundOrbit(float time, void *modelObject);

// Transformation Function: Incoming Rock
QMatrix4x4 transform_IncomingRock(float time, void *modelObject);

#endif // MODELOBJECT_H
