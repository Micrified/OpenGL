#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "model.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QVector3D>
#include <memory>

#include <vector>
#include "vertex.h"
#include "geometry.h"

struct LocationSet {
    GLuint transformLocation;
    GLuint perspectiveLocation;
    GLuint normalLocation;

    // For phong/gouraud shading.
    GLuint lightLocation;
    GLuint materialLocation;

    // For phong/gouraud textures.
    GLuint samplerLocation;
};

class MainView : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

    QOpenGLDebugLogger *debugLogger;
    QTimer timer; // timer used for animation

    /**************************************/

    // The active shader program.
    QOpenGLShaderProgram *activeShaderProgram;

    // The active location set.
    struct LocationSet *activeLocationSet;

    /**************************************/

    // The phong shader and phong LocationSet.
    QOpenGLShaderProgram phongShaderProgram;
    struct LocationSet phongLocationSet;

    // The normal shader and normal LocationSet.
    QOpenGLShaderProgram normalShaderProgram;
    struct LocationSet normalLocationSet;

    // The gouraud shader and gouraud location.
    QOpenGLShaderProgram gouraudShaderProgram;
    struct LocationSet gouraudLocationSet;

    // A light position (x, y, z).
    std::vector<float> lightPosition;

    // Surface material: (ka, kd, ks, n).
    std::vector<float> material;

public:
    enum ShadingMode : GLuint
    {
        PHONG = 0, NORMAL, GOURAUD
    };

    MainView(QWidget *parent = 0);
    ~MainView();

    QVector<quint8> imageToBytes(QImage image);

    // Functions for loading textures.
    void loadTexture(QString file);

    // Functions for widget input events
    void setRotation(int rotateX, int rotateY, int rotateZ);
    void setScale(int scale);
    void setShadingMode(ShadingMode shading);

    // Functions for setting up objects to be rendered.
    void setupVertexObject(GLuint *vbo, GLuint *vao, std::vector<vertex> dataVector);

    // Functions for setting up shaders.
    void initShaderProgram(QString vertPath, QString fragPath, LocationSet *locationSet, QOpenGLShaderProgram *shaderProgram);

protected:
    void initializeGL();
    void resizeGL(int newWidth, int newHeight);
    void paintGL();

    // Functions for keyboard input events
    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);

    // Function for mouse input events
    void mouseDoubleClickEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void wheelEvent(QWheelEvent *ev);

private slots:
    void onMessageLogged( QOpenGLDebugMessage Message );

private:
    void createShaderProgram();

    // *******************************************

    // Mesh vertex count.
    GLuint meshVertexCount;

    // Mesh VBO and VAO.
    GLuint mesh_vbo, mesh_vao;

    // Mesh Translation Matrix.
    QMatrix4x4 meshTranslationMatrix;

    // *******************************************

    // Matrix for mesh transform.
    QMatrix4x4 meshTransform;

    // Matrix for rotation.
    QMatrix4x4 rotationMatrix;

    // Matrix for scaling
    QMatrix4x4 scaleMatrix;

    // Matrix for perspective.
    QMatrix4x4 perspectiveMatrix;

    // Matrix for normal-transform.
    QMatrix3x3 normalTransform;

    // Perspective Location (for sending perspective to shader).
    GLuint perspectiveLocation;

    // Uniform Location (for sending transforms to shader).
    GLuint uniformLocation;

    // Uniform Rotation and Scale Locations (for sending transforms to shader).
    GLuint rotationLocation, scaleLocation;

    // Uniform for normal transforms.
    GLuint normalLocation;

    // *******************************************

    // Texture.
    QVector<quint8> textureBuffer;

    // Texture Pointer.
    GLuint texturePointer;
};

#endif // MAINVIEW_H
