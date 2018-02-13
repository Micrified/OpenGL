#include "mainview.h"
#include "math.h"
#include "vertex.h"
#include <QDateTime>

MainView::MainView(QWidget *parent) : QOpenGLWidget(parent) {
    qDebug() << "MainView constructor";

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
}

MainView::~MainView() {
    debugLogger->stopLogging();

    qDebug() << "MainView destructor";
}

void MainView::initializeGL() {
    qDebug() << ":: Initializing OpenGL";
    initializeOpenGLFunctions();

    debugLogger = new QOpenGLDebugLogger();
    connect( debugLogger, SIGNAL( messageLogged( QOpenGLDebugMessage ) ), this, SLOT( onMessageLogged( QOpenGLDebugMessage ) ), Qt::DirectConnection );

    if ( debugLogger->initialize() ) {
        qDebug() << ":: Logging initialized";
        debugLogger->startLogging( QOpenGLDebugLogger::SynchronousLogging );
        debugLogger->enableMessages();
    }

    QString glVersion;
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

    // Perform custom actions after OpenGL initialization.
    this->setupPostGL();
}

void MainView::setupPostGL() {

    //******************************************************************************
    //                           OpenGL calls placed here
    //******************************************************************************

    // 1. Initializing the QOpenGLShaderProgram class.
    this->shaderProgram = new QOpenGLShaderProgram();
    qDebug() << "QOpenGLShaderProgram: Initialized...";

    // 2. Add the vertex shader, and then the fragment shader.
    this->shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":shaders/vertshader.glsl");
    this->shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":shaders/fragshader.glsl");
     qDebug() << "Vertex and Fragment shader added.";

    // 3. Compile the shader with link.
    this->shaderProgram->link();

    // 4. Activate the shader with bind.
    this->shaderProgram->bind();

    // 5. Initialize the triangle.
    vertex triangle[3];
    triangle[0] = newVertex(-1,-1,1,0,0);
    triangle[1] = newVertex(1,-1,0,1,0);
    triangle[2] = newVertex(0,1,0,0,1);

    // 6. Create VBO: Buffer object names.
    glGenBuffers(3, &(this->vbo));

    // 7. Create VAO: Vertex array.
    glGenVertexArrays(3, &(this->vao));

    // 8. Bind the vertex array.
    glBindVertexArray(this->vao);

    // 9. Bind the buffer.
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

    // 10. Upload the vertex data.
    glBufferData(GL_ARRAY_BUFFER, (3 * sizeof(vertex)), triangle, GL_STATIC_DRAW);

    // 11. Enable Vertex Attribute array (0, and 1).
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // 12. Call .
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)(2 * sizeof(GLfloat)));

}

void MainView::resizeGL(int newWidth, int newHeight) {

    Q_UNUSED(newWidth)
    Q_UNUSED(newHeight)
}

void MainView::paintGL() {

    //******************************************************************************
    //                          Rendering Calls placed here
    //******************************************************************************

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(this->vao);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

    glDrawArrays(GL_TRIANGLES, 0, 3);

}

void MainView::onMessageLogged( QOpenGLDebugMessage Message ) {
    qDebug() << " → Log:" << Message;
}
