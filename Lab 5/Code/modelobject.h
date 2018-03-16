#ifndef MODELOBJECT_H
#define MODELOBJECT_H
#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>
#include <QTimer>
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


    /*
    ********************************************************************************
    *                              Object Animation                                *
    ********************************************************************************
    */

    // Function for setting animation settings.
    void setAnimationFactors (float rotation, float translation);

    // The rotation factor.
    float rotation = 1.0;

    // The translation factor.
    float translation = 0.1;
};

#endif // MODELOBJECT_H
