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

    // 1. Delete the buffers.
    glDeleteBuffers(1, &cube_vbo);
    glDeleteBuffers(1, &py_vbo);
    glDeleteBuffers(1, &sphere_vbo);

    // 2. Delete the vertex arrays.
    glDeleteVertexArrays(1, &cube_vao);
    glDeleteVertexArrays(1, &py_vao);
    glDeleteVertexArrays(1, &sphere_vao);

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

    rotationLocation = shaderProgram.uniformLocation("rotation");
    scaleLocation = shaderProgram.uniformLocation("scale");

    // 4. Initialize the perspective to be able to adjust perspective.
    perspectiveLocation = shaderProgram.uniformLocation("perspective");

    // 5. Setup our transforms & perspective.
    cubeTranslationMatrix.translate({2,0,-6});
    pyramidTranslationMatrix.translate({-2,0,-6});
    sphereTranslationMatrix.translate({0,0,-10});

    // 6. Setup our perspective.
    perspectiveMatrix.perspective(60.0, width()/height(), 0.1, 10.0);

    // *************************************************************************

    // Load in the mesh.
    Model mesh = Model(":models/sphere.obj");
    mesh.unitize();
    // Create the sphere.
    std::vector<vertex> sphere = vectorFrom3D(mesh.getVertices(), 1.0);

    // Set the vertex count.
    sphereVertexCount = sphere.size();

    // Setup the sphere.
    this->setupVertexObject(&sphere_vbo, &sphere_vao, sphere);


    // *************************************************************************

    // Create the cube.
    std::vector<vertex> front = newFace(newVertex(-1, 1, -1, 1, 0, 0),
                                        newVertex(-1, -1, -1, 0, 1, 0),
                                        newVertex(1, -1, -1, 0, 0, 1),
                                        newVertex(1, 1, -1, 1, 1, 0));
    std::vector<vertex> back =  newFace(newVertex(1, 1, 1, 0, 1, 1),
                                        newVertex(1, -1, 1, 1, 0, 1),
                                        newVertex(-1, -1, 1, 0.5, 1, 0.5),
                                        newVertex(-1, 1, 1, 1, 0.5, 1));
    std::vector<vertex> cube = newCube(front, back);

    // Setup the cube.
    this->setupVertexObject(&cube_vbo, &cube_vao, cube);

    // Create the pyramid.
    std::vector<vertex> base = newFace(newVertex(-1, -1, -1, 0.3, 0, 1),
                                      newVertex(1, -1, -1, 0.2, 1, 0.7),
                                      newVertex(1, -1, 1, 0.8, 0, 1),
                                      newVertex(-1, -1, 1, 1, 0.2, 0.6));
    vertex crown = newVertex(0, 1, 0, 1, 0.2, 0.4);
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

    // Setup rotation.
    glUniformMatrix4fv(rotationLocation, 1, GL_FALSE, rotationMatrix.data());

    // Setup scale.
    glUniformMatrix4fv(scaleLocation, 1, GL_FALSE, scaleMatrix.data());

    /******************************************************/

    // Setup Sphere Transformation.
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, sphereTranslationMatrix.data());

    // Scale Sphere Down.

    // Draw sphere.
    glBindVertexArray(this->sphere_vao);
    glDrawArrays(GL_TRIANGLES, 0, sphereVertexCount);

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
    rotationMatrix.setToIdentity();
    rotationMatrix.rotate((float)rotateX, 1.0, 0, 0);
    rotationMatrix.rotate((float)rotateY, 0, 1.0, 0);
    rotationMatrix.rotate((float)rotateZ, 0, 0, 1.0);
    update();
}

void MainView::setScale(int scale)
{
    qDebug() << "Scale changed to " << scale;
    scaleMatrix.setToIdentity();
    scaleMatrix.scale((float)scale/100);
    update();
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
