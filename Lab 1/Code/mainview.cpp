#include "mainview.h"
#include "math.h"

#include <QDateTime>

/**
 * @brief MainView::MainView
 *
 * Constructor of MainView
 *
 * @param parent
 */
MainView::MainView(QWidget *parent) : QOpenGLWidget(parent) {
    qDebug() << "MainView constructor";
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));

}

/**
 * @brief MainView::~MainView
 *
 * Destructor of MainView
 * This is the last function called, before exit of the program
 * Use this to clean up your variables, buffers etc.
 *
 */
MainView::~MainView() {
    debugLogger->stopLogging();

    qDebug() << "MainView destructor";
}

// --- OpenGL initialization

/**
 * @brief MainView::initializeGL
 *
 * Called upon OpenGL initialization
 * Attaches a debugger and calls other init functions
 */
void MainView::initializeGL() {
    qDebug() << ":: Initializing OpenGL";
    initializeOpenGLFunctions();

    debugLogger = new QOpenGLDebugLogger();
    connect( debugLogger, SIGNAL( messageLogged( QOpenGLDebugMessage ) ),
             this, SLOT( onMessageLogged( QOpenGLDebugMessage ) ), Qt::DirectConnection );

    if ( debugLogger->initialize() ) {
        qDebug() << ":: Logging initialized";
        debugLogger->startLogging( QOpenGLDebugLogger::SynchronousLogging );
        debugLogger->enableMessages();
    }

    QString glVersion;
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable backface culling
    glEnable(GL_CULL_FACE);

    // Default is GL_LESS
    glDepthFunc(GL_LEQUAL);

    // Set the color of the screen to be black on clear (new frame)
    glClearColor(0.2f, 0.5f, 0.7f, 0.0f);

    createShaderProgram();
}

void MainView::setupVertexObject(GLuint *vbo, GLuint *vao, std::vector<vertex> dataVector) {

    // 1. Initialize VBO and VAO.
    glGenBuffers(1, vbo);
    glGenVertexArrays(1, vao);

    // 2. Bind the vertex array and buffer.
    glBindVertexArray(*vao);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);

    // 3. Enable both attributes of vertex coord(x,y,z) and color(r,g,b).
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // 4. Describe the data layout.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)(3 * sizeof(GLfloat)));

    // 5. Upload the data: Use &<name>[0] for std::vector data.
    glBufferData(GL_ARRAY_BUFFER, (dataVector.size() * sizeof(vertex)), &dataVector[0], GL_STATIC_DRAW);
}

void MainView::createShaderProgram()
{
    // 1. Add vertex and fragment shaders.
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader.glsl");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader.glsl");

    // 2. Compile the shader with link. Then activate the shader with bind.
    shaderProgram.link();
    shaderProgram.bind();

    // 3. Initialize the uniformLocation to be able to do transformations.
    uniformLocation = shaderProgram.uniformLocation("modelTransform");

    // 4. Initialize the perspective to be able to adjust perspective.
    perspectiveLocation = shaderProgram.uniformLocation("perspective");

    // 5. Setup our transforms & perspective.
    cubeTranslationMatrix.translate({-2,1,-6});
    pyramidTranslationMatrix.translate({-2,0,-6});

    // 6. Setup our perspective.
    perspectiveMatrix.perspective(60.0, width()/height(), 0.1, 10.0);

    // *************************************************************************

    // Create the cube.
    std::vector<vertex> front = newFace(newVertex(-0.5, 0.5, -0.5, 1, 0, 0),
                                        newVertex(-0.5, -0.5, -0.5, 0, 1, 0),
                                        newVertex(0.5, -0.5, -0.5, 0, 0, 1),
                                        newVertex(0.5, 0.5, -0.5, 1, 1, 0));
    std::vector<vertex> back =  newFace(newVertex(0.5, 0.5, 0.5, 0, 1, 1),
                                        newVertex(0.5, -0.5, 0.5, 1, 0, 1),
                                        newVertex(-0.5, -0.5, 0.5, 0.5, 1, 0.5),
                                        newVertex(-0.5, 0.5, 0.5, 1, 0.5, 1));
    std::vector<vertex> cube = newCube(front, back);

    // Setup the cube.
    this->setupVertexObject(&cube_vbo, &cube_vao, cube);

    // Create the pyramid.
    std::vector<vertex> base = newFace(newVertex(-0.5, -0.5, -0.5, 0.3, 0, 1),
                                      newVertex(0.5, -0.5, -0.5, 0.2, 1, 0.7),
                                      newVertex(0.5, -0.5, 0.5, 0.8, 0, 1),
                                      newVertex(-0.5, -0.5, 0.5, 1, 0.2, 0.6));
    vertex crown = newVertex(0, 0.75, 0, 1, 0.2, 0.4);
    std::vector<vertex> pyramid = newPyramid(base, crown);

    // Setup the pyramid.
    this->setupVertexObject(&py_vbo, &py_vao, pyramid);
}

// --- OpenGL drawing

/**
 * @brief MainView::paintGL
 *
 * Actual function used for drawing to the screen
 *
 */
void MainView::paintGL() {

    // Clear the screen before rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.bind();

    // Setup perspective.
    glUniformMatrix4fv(perspectiveLocation, 1, GL_FALSE, perspectiveMatrix.data());

    /******************************************************/

    // Setup Cube Transformation.
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, cubeTranslationMatrix.data());

    // Draw Cube
    glBindVertexArray(this->cube_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);


    /******************************************************/

    // Setup Pyramid Transformation.
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, pyramidTranslationMatrix.data());

    // Draw Pyramid
    glBindVertexArray(this->py_vao);
    glDrawArrays(GL_TRIANGLES, 0, 18);

    shaderProgram.release();
}

/**
 * @brief MainView::resizeGL
 *
 * Called upon resizing of the screen
 *
 * @param newWidth
 * @param newHeight
 */
void MainView::resizeGL(int newWidth, int newHeight) 
{
    // TODO: Update projection to fit the new aspect ratio
    perspectiveMatrix.setToIdentity();
    perspectiveMatrix.perspective(60.0, ((float)newWidth/(float)newHeight), 0.1, 10.0);

    qDebug() << "Perspective changed to (" << newWidth << "," << newHeight << ")";
}

// --- Public interface

void MainView::setRotation(int rotateX, int rotateY, int rotateZ)
{
    qDebug() << "Rotation changed to (" << rotateX << "," << rotateY << "," << rotateZ << ")";
    Q_UNIMPLEMENTED();
}

void MainView::setScale(int scale)
{
    qDebug() << "Scale changed to " << scale;
    Q_UNIMPLEMENTED();
}

void MainView::setShadingMode(ShadingMode shading)
{
    qDebug() << "Changed shading to" << shading;
    Q_UNIMPLEMENTED();
}

// --- Private helpers

/**
 * @brief MainView::onMessageLogged
 *
 * OpenGL logging function, do not change
 *
 * @param Message
 */
void MainView::onMessageLogged( QOpenGLDebugMessage Message ) {
    qDebug() << " → Log:" << Message;
}
