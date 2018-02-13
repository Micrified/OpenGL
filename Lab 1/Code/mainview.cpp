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

void MainView::createShaderProgram()
{
    // 1. Initialize shader program.
    //shaderProgram = new QOpenGLShaderProgram();

    // 2. Add vertex and fragment shaders.
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           ":/shaders/vertshader.glsl");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           ":/shaders/fragshader.glsl");

    // 3. Compile with link.
    shaderProgram.link();

    // 4. Activate shader with bind.
    shaderProgram.bind();

    // 5. Initialize the cube.
    vertex cube[36], *cp = cube;

    vertex a1 = newVertex(-0.5,-0.5,-0.5,1,0,0);
    vertex a2 = newVertex(0.5,-0.5,-0.5,1,0,0);
    vertex a3 = newVertex(-0.5,0.5,-0.5,1,0,0);
    vertex a4 = newVertex(0.5,0.5,-0.5,1,0,0);

    vertex b1 = newVertex(-1,-1,1,0,1,0);
    vertex b2 = newVertex(1,-1,1,0,1,0);
    vertex b3 = newVertex(-1,1,1,0,1,0);
    vertex b4 = newVertex(1,1,1,0,1,0);

    setCube(cp, a1,a2,a3,a4, b1,b2,b3,b4);

    // 6. Create VBO.
    glGenBuffers(1, &(this->vbo));

    // 7. Create VAO.
    glGenVertexArrays(1, &(this->vao));

    // 8. Bind vertex array.
    glBindVertexArray(this->vao);

    // 9. Bind the buffer.
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

    // 10. Upload the vertex data.
    glBufferData(GL_ARRAY_BUFFER, (36 * sizeof(vertex)), cp, GL_STATIC_DRAW);

    // 11. Enable vertex attribute arrays (position, color).
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // 12. Call.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)(3 * sizeof(GLfloat)));
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

    // Draw here
    glDrawArrays(GL_TRIANGLES, 0, 36);

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
    Q_UNUSED(newWidth)
    Q_UNUSED(newHeight)
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
