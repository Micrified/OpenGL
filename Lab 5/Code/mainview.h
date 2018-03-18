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
#include "modelobject.h"

// Number of scene objects.
#define N_SCENE_OBJECTS     4

/*
********************************************************************************
*                                 Data Types                                   *
********************************************************************************
*/

/* Contains all pointers to a shader program's vertex buffers. */
typedef struct {

    /* CATEGORY: Transforms. */
    GLuint vertexTransformLocation;     // Pointer to vertex transform buffer.
    GLuint normalTransformLocation;     // Pointer to normal transform buffer.
    GLuint perspectiveLocation;         // Pointer to perspective transform buffer.

    /* CATEGORY: Scene */
    GLuint sceneTransformLocation;      // Pointer to scene rotation buffer.

    /* CATEGORY: Lighting & Material. */
    GLuint lightCoordinateLocation;     // Pointer to light-source coordinate buffer.
    GLuint materialLocation;            // Pointer to material data buffer.

    /* CATEGORY: Textures. */
    GLuint samplerLocation;             // Pointer to texture data buffer.

} ShaderLocationSet;


/*
********************************************************************************
*                              Class Declaration                               *
********************************************************************************
*/


class MainView : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

    QOpenGLDebugLogger *debugLogger;
    QTimer timer; // timer used for animation

    /*
    ****************************************************************************
    *                                Shaders                                   *
    ****************************************************************************
    */

    // The Normal shader program.
    QOpenGLShaderProgram normalShaderProgram;

    // The Phong shader program.
    QOpenGLShaderProgram phongShaderProgram;

    // The Gouraud shader program.
    QOpenGLShaderProgram gouraudShaderProgram;

    /*
    ****************************************************************************
    *                          Shader Buffer Pointers                          *
    ****************************************************************************
    */

    // The set of buffer-pointers for the normal shader program.
    ShaderLocationSet normalShaderLocationSet;

    // The set of buffer-pointers for the phong shader program.
    ShaderLocationSet phongShaderLocationSet;

    // The set of buffer-pointers for the gouraud shader program.
    ShaderLocationSet gouraudShaderLocationSet;

    /*
    ****************************************************************************
    *                         Active Shader Information                        *
    ****************************************************************************
    */

    // A pointer to the currently active shader program.
    QOpenGLShaderProgram *activeShaderProgramPointer;

    // A pointer to the currently active shader location set.
    ShaderLocationSet *activeLocationSetPointer;

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

    /*
    ****************************************************************************
    *                          Custom Utility Methods                          *
    ****************************************************************************
    */

    // Method for converting an texture image to bytes.
    QVector<quint8> imageToBytes(QImage image);

    // Method for loading in a texture. Uses texturePointer to store texture.
    void loadTexture (QString file, GLuint *texturePointer);

    // Method for setting up a shader program.
    void setupShaderProgram (const QString &, const QString &, QOpenGLShaderProgram *, ShaderLocationSet *);

    // Method for setting up objects to be rendered.
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

    /*
    ********************************************************************************
    *                                Scene Objects                                 *
    ********************************************************************************
    */

    // Objects in the scene.
    ModelObject sceneObjects[N_SCENE_OBJECTS];

    /*
    ********************************************************************************
    *                              Scene Object Data                               *
    ********************************************************************************
    */

    // Lighting coordinate.
    std::vector<float> lightCoordinateVector;

    /*
    ********************************************************************************
    *                            Scene Object Transform                            *
    ********************************************************************************
    */

    // Matrix representing the current perspective.
    QMatrix4x4 perspectiveMatrix;

    // Matrix representing the current rotation of the scene.
    QMatrix4x4 sceneRotationMatrix;

    // Matrix representing the translation of the scene.
    QMatrix4x4 sceneTranslationMatrix;

    // Matrix representing the scaling of the scene.
    QMatrix4x4 sceneScaleMatrix;

    /*
    ********************************************************************************
    *                                  Animation                                   *
    ********************************************************************************
    */

    float time = 0;

};

#endif // MAINVIEW_H
