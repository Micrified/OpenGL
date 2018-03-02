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
    glDeleteBuffers(1, &mesh_vbo);

    // 2. Delete the vertex arrays.
    glDeleteVertexArrays(1, &mesh_vao);

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

QVector<quint8> MainView::imageToBytes(QImage image) {
    // needed since (0,0) is bottom left in OpenGL
    QImage im = image.mirrored();
    QVector<quint8> pixelData;
    pixelData.reserve(im.width()*im.height()*4);

    for (int i = 0; i != im.height(); ++i) {
        for (int j = 0; j != im.width(); ++j) {
            QRgb pixel = im.pixel(j,i);

            // pixel is of format #AARRGGBB (in hexadecimal notation)
            // so with bitshifting and binary AND you can get
            // the values of the different components
            quint8 r = (quint8)((pixel >> 16) & 0xFF); // Red component
            quint8 g = (quint8)((pixel >> 8) & 0xFF); // Green component
            quint8 b = (quint8)(pixel & 0xFF); // Blue component
            quint8 a = (quint8)((pixel >> 24) & 0xFF); // Alpha component

            // Add them to the Vector
            pixelData.append(r);
            pixelData.append(g);
            pixelData.append(b);
            pixelData.append(a);
        }
    }
    return pixelData;
}


// Functions for loading textures.
void MainView::loadTexture(QString file) {

    // Load in the image. Convert to byte-buffer.
    QImage image = QImage(file);
    textureBuffer = imageToBytes(image);

    // Reserve a pointer to the texture.
    glGenTextures(1, &texturePointer);

    // Bind our texture to our target pointer.
    glBindTexture(GL_TEXTURE_2D, texturePointer);

    // Set texture parameters.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_TEXTURE_WRAP_T);

    // Export this bad boi.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer.data());
}

void MainView::setupVertexObject(GLuint *vbo, GLuint *vao, std::vector<vertex> dataVector) {

    // 1. Initialize VBO and VAO.
    glGenBuffers(1, vbo);
    glGenVertexArrays(1, vao);

    // 2. Bind the vertex array and buffer.
    glBindVertexArray(*vao);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);

    // 3. Enable both attributes of vertex coord(x,y,z) and color(r,g,b) or normal (nx,ny,nz)
    //    and texture coord (x,y).
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // 4. Describe the data layout: {(x,y,z) + ((r,g,b) | (nx,ny,nz)) + (x,y)}.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)(3 * sizeof(GLfloat)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)(6 * sizeof(GLfloat)));

    // 5. Upload the data: Use &<name>[0] for std::vector data.
    glBufferData(GL_ARRAY_BUFFER, (dataVector.size() * sizeof(vertex)), &dataVector[0], GL_STATIC_DRAW);
}

void MainView:: initShaderProgram(QString vertPath, QString fragPath, LocationSet *locationSet, QOpenGLShaderProgram *shaderProgram) {

    // 1. Add vertex and fragment shaders.
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, vertPath);
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, fragPath);

    // 2. Compile the shader with link. Then activate the shader with bind.
    shaderProgram->link();
    shaderProgram->bind();

    // 3. Initialize: transform, rotation, scale, perspective, and normal
    locationSet->transformLocation = shaderProgram->uniformLocation("modelTransformUniform");
    locationSet->perspectiveLocation = shaderProgram->uniformLocation("perspectiveUniform");
    locationSet->normalLocation = shaderProgram->uniformLocation("normalTransformUniform");
    locationSet->lightLocation = shaderProgram->uniformLocation("lightCoordinateUniform");
    locationSet->materialLocation = shaderProgram->uniformLocation("materialUniform");

    // 4. Initialize: Sampler.
    locationSet->samplerLocation = shaderProgram->uniformLocation("samplerUniform");

}

void MainView::createShaderProgram()
{
    // Initialize all shader programs.
    initShaderProgram(":/shaders/vertshader_phong.glsl", ":/shaders/fragshader_phong.glsl", &phongLocationSet, &phongShaderProgram);
    initShaderProgram(":/shaders/vertshader_normal.glsl", ":/shaders/fragshader_normal.glsl", &normalLocationSet, &normalShaderProgram);
    initShaderProgram(":/shaders/vertshader_gouraud.glsl", ":/shaders/fragshader_gouraud.glsl", &gouraudLocationSet, &gouraudShaderProgram);

    // Setup our transforms & perspective.
    meshTranslationMatrix.translate({0,-1,-3});

    // Set light location.
    std::vector<float> light(3);
    light[0] = 0.0; light[1] = 0.0; light[2] = 4.0;
    lightPosition = light;

    // Setup material: (ka, kd, ks, n).
    std::vector<float> material(4);
    material[0] = 0.1; material[1] = 0.5; material[2] = 0.8; material[3] = 64;
    this->material = material;

    // Setup our perspective.
    perspectiveMatrix.perspective(60.0, width()/height(), 0.1, 10.0);

    // *************************************************************************

    // Load in the mesh.
    Model meshModel = Model(":models/cat.obj");
    meshModel.unitize();

    // Create the data-vector.
    std::vector<vertex> mesh = vectorFrom3D(meshModel.getVertices(), meshModel.getNormals(), meshModel.getTextureCoords());

    // Set the vertex count.
    meshVertexCount = mesh.size();

    // Setup the data-vector.
    this->setupVertexObject(&mesh_vbo, &mesh_vao, mesh);


    // *************************************************************************

    // Setup model textures.
    loadTexture(":/textures/cat_diff.png");
    qDebug() << "Setup textures...\n";


    // *************************************************************************

    // Setup active shader and location set.
    activeShaderProgram = &phongShaderProgram;
    activeLocationSet = &phongLocationSet;

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

    activeShaderProgram->bind();

    // Apply texture.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texturePointer);

    // Setup perspective.
    glUniformMatrix4fv(activeLocationSet->perspectiveLocation, 1, GL_FALSE, perspectiveMatrix.data());

    // Setup model transform.
    meshTransform = (meshTranslationMatrix * scaleMatrix * rotationMatrix);

    // Setup mesh transformation.
    glUniformMatrix4fv(activeLocationSet->transformLocation, 1, GL_FALSE, meshTransform.data());

    // Setup normal transformation.
    normalTransform = meshTransform.normalMatrix();

   // Apply normal transformation.
    glUniformMatrix3fv(activeLocationSet->normalLocation, 1, GL_FALSE, normalTransform.data());

    // Apply light and material data.
    glUniform3fv(activeLocationSet->lightLocation, 1, lightPosition.data());
    glUniform4fv(activeLocationSet->materialLocation, 1, material.data());

    /******************************************************/

    // Draw mesh.
    glBindVertexArray(this->mesh_vao);
    glDrawArrays(GL_TRIANGLES, 0, meshVertexCount);

    activeShaderProgram->release();
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
    if (shading == 0) {
        activeShaderProgram = &phongShaderProgram;
        activeLocationSet = &phongLocationSet;
    } else if (shading == 1) {
        activeShaderProgram = &normalShaderProgram;
        activeLocationSet = &normalLocationSet;
    } else {
        activeShaderProgram = &gouraudShaderProgram;
        activeLocationSet = &gouraudLocationSet;
    }
    qDebug() << "Changed shading to" << shading;
    update();
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
