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

    // Delete all buffers, and vertex array.
    for (int i = 0; i < N_SCENE_OBJECTS; i++) {
        glDeleteBuffers(1, &sceneObjects[i].mesh_vbo);          // Free VBO
        glDeleteVertexArrays(1, &sceneObjects[i].mesh_vao);     // Free VAO
        glDeleteTextures(1, &sceneObjects[i].texturePointer);   // Free textures
    }

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
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Create Shader Program.
    createShaderProgram();

    // Starting the refresh timer
    timer.start(1000.0 / 60.0);
}

/*
********************************************************************************
*                            Custom Utility Programs                           *
********************************************************************************
*/

// Method for converting an texture image to bytes.
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

// Method for loading in a texture. Uses global texturePointer to store texture.
void MainView::loadTexture (QString file, GLuint *texturePointer) {

    // Generate a texture.
    glGenTextures(1, texturePointer);

    // Prepare to use the texture.
    glBindTexture(GL_TEXTURE_2D, *texturePointer);

    // Setup texture interpretation.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Load in the texture and convert to bytes.
    QImage image = QImage(file);
    QVector<quint8> imageBytes = imageToBytes(image);

    // Upload the data.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBytes.data());
}

// Method for setting up a shader program.
void MainView::setupShaderProgram (const QString &vertexShaderPath, const QString &fragmentShaderPath, QOpenGLShaderProgram *shaderProgramPointer, ShaderLocationSet *locationSetPointer) {

    // Initialize the vertex and fragment shaders.
    shaderProgramPointer->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderPath);
    shaderProgramPointer->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderPath);

    // Compile and Activate the shader.
    shaderProgramPointer->link();
    shaderProgramPointer->bind();

    // Initialize and assign all buffer-pointers to the location-set.
    locationSetPointer->vertexTransformLocation = shaderProgramPointer->uniformLocation("vertexTransformUniform");
    locationSetPointer->normalTransformLocation = shaderProgramPointer->uniformLocation("normalTransformUniform");
    locationSetPointer->perspectiveLocation = shaderProgramPointer->uniformLocation("perspectiveUniform");

    // Initialize and asssign all scene buffer-pointers.
    locationSetPointer->sceneTransformLocation = shaderProgramPointer->uniformLocation("sceneTransformUniform");

    // Initialize the light coordinate, and the material properties.
    locationSetPointer->lightCoordinateLocation = shaderProgramPointer->uniformLocation("lightCoordinateUniform");
    locationSetPointer->materialLocation = shaderProgramPointer->uniformLocation("materialUniform");

    // Initialize sampler location (for textures).
    locationSetPointer->samplerLocation = shaderProgramPointer->uniformLocation("samplerUniform");
}

// Method for setting up objects to be rendered.
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
    glEnableVertexAttribArray(2);

    // 4. Describe the data layout.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)(3 * sizeof(GLfloat)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)(6 * sizeof(GLfloat)));

    // 5. Upload the data: Use &<name>[0] for std::vector data.
    glBufferData(GL_ARRAY_BUFFER, (dataVector.size() * sizeof(vertex)), &dataVector[0], GL_STATIC_DRAW);
}

void MainView::createShaderProgram()
{
    qDebug() << "Creating Shader Program";

    /*
    ****************************************************************************
    *                            Setup All Shaders                             *
    ****************************************************************************
    */

    // Setup the normal shader.
    setupShaderProgram(":/shaders/vertshader_normal.glsl", ":/shaders/fragshader_normal.glsl",
                       &normalShaderProgram, &normalShaderLocationSet);

    // Setup the phong shader.
    setupShaderProgram(":/shaders/vertshader_phong.glsl", ":/shaders/fragshader_phong.glsl",
                       &phongShaderProgram, &phongShaderLocationSet);

    // Setup the gouraud shader.
    setupShaderProgram(":/shaders/vertshader_gouraud.glsl", ":/shaders/fragshader_gouraud.glsl",
                       &gouraudShaderProgram, &gouraudShaderLocationSet);

    /*
    ****************************************************************************
    *                  Setup Initial Model/Perspective Transforms              *
    ****************************************************************************
    */

    // DEFAULT: Perspective.
    perspectiveMatrix.perspective(60.0, width()/height(), 0.1, 50.0);

    // DEFAULT: Scene translation.
    sceneTranslationMatrix.translate(0, 0, -10.0);

    /*
    ****************************************************************************
    *                              Load-In Model(s)                            *
    ****************************************************************************
    */

    ModelObject             *modelObject;
    std::vector<vertex>     mesh;

    // --------------------------- Object 1: Earth ----------------------------

    // Set local modelObject.
    modelObject = &sceneObjects[0];

    // Load in model and unitize.
    Model model = Model(":models/sphere.obj"); model.unitize();

    // Translate model mesh to vector.
    mesh = vectorFrom3D(model.getVertices(), model.getNormals(), model.getTextureCoords());

    // Set the model vertex count.
    modelObject->meshVertexCount = mesh.size();

    // Set model translation.
    modelObject->translationMatrix.translate(0, -1.0, 0.0);

    // Scale model.
    modelObject->scaleMatrix.scale(2.0);

    // Set animation factors (rotation speeds: rx, ry, rz, translation speeds: tx, ty, tz).
    modelObject->setAnimationFactors(0, 1.0, 0, 0, 0, 0);

    // Prepare model to be shown in scene.
    setupVertexObject(&modelObject->mesh_vbo, &modelObject->mesh_vao, mesh);

    // Load texture.
    loadTexture(":/textures/earth.png", &modelObject->texturePointer);

    // Load material.
    modelObject->materialVector = std::vector<float>{0.5, 0.9, 0.9, 64.0};

    // Set transform function.
    modelObject->transform = transform_Earth;


    // ---------------------------- Object 2: Moon -----------------------------

    // Set local modelObject.
    modelObject = &sceneObjects[1];

    // Load in model and unitize.
    model = Model(":models/sphere.obj"); model.unitize();

    // Translate model mesh to vector.
    mesh = vectorFrom3D(model.getVertices(), model.getNormals(), model.getTextureCoords());

    // Set the model vertex count.
    modelObject->meshVertexCount = mesh.size();

    // Set model translation.
    modelObject->translationMatrix.translate(0, -1.0, 0.0);

    // Scale model.
    modelObject->scaleMatrix.scale(0.75);

    // Set animation factors (rotation speeds: rx, ry, rz, translation speeds: tx, ty, tz).
    modelObject->setAnimationFactors(0.0, 1.0, 0.0, 0.125, 0.0, 0.125);
    modelObject->radius = 6;

    // Prepare model to be shown in scene.
    setupVertexObject(&modelObject->mesh_vbo, &modelObject->mesh_vao, mesh);

    // Load texture.
    loadTexture(":/textures/moon.png", &modelObject->texturePointer);

    // Load material.
    modelObject->materialVector = std::vector<float>{0.5, 0.9, 0.9, 64.0};

    // Set transform function.
    modelObject->transform = transform_Moon;

    // ------------------------- Object 3: Incoming-Rock --------------------------

    // Set local modelObject.
    modelObject = &sceneObjects[2];

    // Load in model and unitize.
    model = Model(":models/rock.obj"); model.unitize();

    // Translate model mesh to vector.
    mesh = vectorFrom3D(model.getVertices(), model.getNormals(), model.getTextureCoords());

    // Set the model vertex count.
    modelObject->meshVertexCount = mesh.size();

    // Set model translation.
    modelObject->translationMatrix.translate(0.0, 2.0, -20.0);

    // Scale model.
    modelObject->scaleMatrix.scale(0.5);

    // Set animation factors (rotation speeds: rx, ry, rz, translation speeds: tx, ty, tz).
    modelObject->setAnimationFactors(15.0, 15.0, 15.0, 0.0, 0.0, 2.0);

    // Prepare model to be shown in scene.
    setupVertexObject(&modelObject->mesh_vbo, &modelObject->mesh_vao, mesh);

    // Load texture.
    loadTexture(":/textures/rock.png", &modelObject->texturePointer);

    // Load material.
    modelObject->materialVector = std::vector<float>{0.5, 0.9, 0.9, 64.0};

    // Set transform function.
    modelObject->transform = transform_IncomingRock;

    // ---------------------------- Object 4: Orbiting-Rock -----------------------------

    // Set local modelObject.
    modelObject = &sceneObjects[3];

    // Load in model and unitize.
    model = Model(":models/rock.obj"); model.unitize();

    // Translate model mesh to vector.
    mesh = vectorFrom3D(model.getVertices(), model.getNormals(), model.getTextureCoords());

    // Set the model vertex count.
    modelObject->meshVertexCount = mesh.size();

    // Set model translation.
    modelObject->translationMatrix.translate(0, -1.0, 0.0);

    // Scale model.
    modelObject->scaleMatrix.scale(0.25);

    // Set animation factors (rotation speeds: rx, ry, rz, translation speeds: tx, ty, tz).
    modelObject->setAnimationFactors(15.0, 15.0, 15.0, 0.125, 0.0, 0.125);
    modelObject->radius = 6;

    // Prepare model to be shown in scene.
    setupVertexObject(&modelObject->mesh_vbo, &modelObject->mesh_vao, mesh);

    // Load texture.
    loadTexture(":/textures/rock.png", &modelObject->texturePointer);

    // Load material.
    modelObject->materialVector = std::vector<float>{0.5, 0.9, 0.9, 64.0};

    // Set transform function.
    modelObject->transform = transform_CompoundOrbit;

    /*
    ****************************************************************************
    *                    Set Material and Lighting Properties                  *
    ****************************************************************************
    */

    // Set the location of the light.
    lightCoordinateVector = std::vector<float>{2.0, 2.0, 2.0};

    /*
    ****************************************************************************
    *                       Setup Initial Shader Program                       *
    ****************************************************************************
    */

    activeShaderProgramPointer = &phongShaderProgram;
    activeLocationSetPointer = &phongShaderLocationSet;
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

    // BIND: Active Shader Program.
    activeShaderProgramPointer->bind();

    // COMPUTE: Scene transform.
    QMatrix4x4 sceneTransformMatrix = sceneTranslationMatrix * sceneRotationMatrix * sceneScaleMatrix;

    for(int i = 0; i < N_SCENE_OBJECTS; i++){

        // EXTRACT OBJECT TO BE RENDERED.
        ModelObject *modelObject = &sceneObjects[i];
        GLuint texturePointer = modelObject->texturePointer;
        std::vector<float> materialVector = modelObject->materialVector;
        GLuint mesh_vao = modelObject->mesh_vao;
        int meshVertexCount = modelObject->meshVertexCount;

        // Bind textures.
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texturePointer);

        // Obtain Active Locations.
        GLuint vertexTransformLocation  = activeLocationSetPointer->vertexTransformLocation;
        GLuint normalTransformLocation  = activeLocationSetPointer->normalTransformLocation;
        GLuint perspectiveLocation      = activeLocationSetPointer->perspectiveLocation;
        GLuint sceneTransformLocation   = activeLocationSetPointer->sceneTransformLocation;
        GLuint lightCoordinateLocation  = activeLocationSetPointer->lightCoordinateLocation;
        GLuint materialLocation         = activeLocationSetPointer->materialLocation;

        // SET: Scene transform.
        glUniformMatrix4fv(sceneTransformLocation, 1, GL_FALSE, sceneTransformMatrix.data());

        // SET: Lighting Coordinate.
        glUniform3fv(lightCoordinateLocation, 1, lightCoordinateVector.data());

        // SET: Material
        glUniform4fv(materialLocation, 1, materialVector.data());

        // UPDATE: time
        time += 1.0 / 60.0;

        // COMPUTE: Transform (sceneTransform • translation • scale • rotation).
        QMatrix4x4 transformMatrix = sceneTransformMatrix * modelObject->transformMatrix(time);

        // COMPUTE: Normal-Transform. (CHECK THIS IS CORRECT?)
        QMatrix3x3 normalTransformMatrix = transformMatrix.normalMatrix();

        // SET: Transform.
        glUniformMatrix4fv(vertexTransformLocation, 1, GL_FALSE, transformMatrix.data());

        // SET: Perspective.
        glUniformMatrix4fv(perspectiveLocation, 1, GL_FALSE, perspectiveMatrix.data());

        // SET: Normal-Transform.
        glUniformMatrix3fv(normalTransformLocation, 1, GL_FALSE, normalTransformMatrix.data());

        // DRAW: Model Mesh.
        glBindVertexArray(mesh_vao);
        glDrawArrays(GL_TRIANGLES, 0, meshVertexCount);

    }
    // RELEASE: Active Shader Program.
    activeShaderProgramPointer->release();
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
    perspectiveMatrix.perspective(60.0, ((float)newWidth/(float)newHeight), 0.1, 50.0);

    qDebug() << "Perspective changed to (" << newWidth << "," << newHeight << ")";
}

// --- Public interface

void MainView::setRotation(int rotateX, int rotateY, int rotateZ)
{
    qDebug() << "Rotation changed to (" << rotateX << "," << rotateY << "," << rotateZ << ")";
    sceneRotationMatrix.setToIdentity();
    sceneRotationMatrix.rotate((float)rotateX, 1.0, 0, 0);
    sceneRotationMatrix.rotate((float)rotateY, 0, 1.0, 0);
    sceneRotationMatrix.rotate((float)rotateZ, 0, 0, 1.0);

    update();
}

void MainView::setScale(int scale)
{
    qDebug() << "Scale changed to " << scale;
    sceneScaleMatrix.setToIdentity();
    sceneScaleMatrix.scale((float)scale/100);
    update();
}

void MainView::setShadingMode(ShadingMode shading)
{
    // Display Shading Modes:
    // 0 = phong, 1 = normal, 2 = gouraud.
    if (shading == 0) {
        activeShaderProgramPointer = &phongShaderProgram;
        activeLocationSetPointer = &phongShaderLocationSet;
        qDebug() << "Shading Mode = Phong\n";
    } else if (shading == 1) {
        activeShaderProgramPointer = &normalShaderProgram;
        activeLocationSetPointer = &normalShaderLocationSet;
        qDebug() << "Shading Mode = Normal\n";
    } else {
        activeShaderProgramPointer = &gouraudShaderProgram;
        activeLocationSetPointer = &gouraudShaderLocationSet;
        qDebug() << "Shading Mode = Gouraud\n";
    }

    // Update the scene.
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
