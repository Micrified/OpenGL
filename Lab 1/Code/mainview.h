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

class MainView : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

    QOpenGLDebugLogger *debugLogger;
    QTimer timer; // timer used for animation

    // The shader program.
    QOpenGLShaderProgram shaderProgram;

public:
    enum ShadingMode : GLuint
    {
        PHONG = 0, NORMAL, GOURAUD
    };

    MainView(QWidget *parent = 0);
    ~MainView();

    // Functions for widget input events
    void setRotation(int rotateX, int rotateY, int rotateZ);
    void setScale(int scale);
    void setShadingMode(ShadingMode shading);

    // Functions for setting up objects to be rendered.
    void setupVertexObject(GLuint *vbo, GLuint *vao, std::vector<vertex> dataVector);

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

    // Cube VBO & VAO.
    GLuint cube_vbo, cube_vao;

    // Cube Translation Matrix.
    QMatrix4x4 cubeTranslationMatrix;

    // *******************************************

    // Pyramid VBO & VAO.
    GLuint py_vbo, py_vao;

    // Pyramid Translation Matrix.
    QMatrix4x4 pyramidTranslationMatrix;

    // *******************************************

    // Matrix for perspective.
    QMatrix4x4 perspectiveMatrix;

    // Perspective Location (for sending perspective to shader).
    GLuint perspectiveLocation;

    // Uniform Location (for sending transforms to shader).
    GLuint uniformLocation;
};

#endif // MAINVIEW_H
