#include "modelobject.h"


/*
 ********************************************************************************
 *                        ModelObject Instance Functions                        *
 ********************************************************************************
*/

ModelObject::ModelObject()
{}

// Function for setting object transforms.
void ModelObject::setTransform(QMatrix4x4 translationMatrix, QMatrix4x4 rotationMatrix, QMatrix4x4 scaleMatrix) {
    this->translationMatrix = translationMatrix;
    this->rotationMatrix = rotationMatrix;
    this->scaleMatrix = scaleMatrix;
}

// Function for setting animation settings.
void ModelObject::setAnimationFactors (float rotx, float roty, float rotz, float transx, float transy, float transz) {
    this->rotation[0] = rotx; this->rotation[1] = roty; this->rotation[2] = rotz;
    this->translation[0] = transx; this->translation[1] = transy; this->translation[2] = transz;
}

// Function to get the transform matrix at a given time
QMatrix4x4 ModelObject::transformMatrix(float t) {
    return this->transform(t, this);
}

/*
 ********************************************************************************
 *                            Animation Functions                               *
 ********************************************************************************
*/

// Transformation Function: Earth
QMatrix4x4 transform_Earth(float time, void *modelObject) {
    ModelObject *model = (ModelObject *)modelObject;
    QMatrix4x4 translationMatrixCopy = model->translationMatrix;
    QMatrix4x4 rotationMatrixCopy = model->rotationMatrix;

    float x = sin(time * model->translation[0]);
    float y = sin(time * model->translation[1]);
    float z = sin(time * model->translation[2]);

    translationMatrixCopy.translate(x, y, z);

    rotationMatrixCopy.rotate(time * model->rotation[0], 1, 0, 0);
    rotationMatrixCopy.rotate(time * model->rotation[1], 0, 1, 0);
    rotationMatrixCopy.rotate(time * model->rotation[2], 0, 0, 1);

    return translationMatrixCopy * model->scaleMatrix * rotationMatrixCopy;
}

// Transformation Function: Moon
QMatrix4x4 transform_Moon(float time, void *modelObject) {
    ModelObject *model = (ModelObject *)modelObject;
    QMatrix4x4 translationMatrixCopy = model->translationMatrix;
    QMatrix4x4 rotationMatrixCopy = model->rotationMatrix;

    float x = model->radius * sin(time * model->translation[0]);
    float y = model->translation[1];
    float z = model->radius * cos(time * model->translation[2]);

    translationMatrixCopy.translate(x, y, z);

    rotationMatrixCopy.rotate(time * model->rotation[0], 1, 0, 0);
    rotationMatrixCopy.rotate(time * model->rotation[1], 0, 1, 0);
    rotationMatrixCopy.rotate(time * model->rotation[2], 0, 0, 1);

    return translationMatrixCopy * model->scaleMatrix * rotationMatrixCopy;
}

// Transformation Function: Compound Orbit
QMatrix4x4 transform_CompoundOrbit(float time, void *modelObject) {
    ModelObject *model = (ModelObject *)modelObject;
    QMatrix4x4 translationMatrixCopy = model->translationMatrix;
    QMatrix4x4 rotationMatrixCopy = model->rotationMatrix;

    float x1 = model->radius * sin(time * model->translation[0]);
    float y1 = model->translation[1];
    float z1 = model->radius * cos(time * model->translation[2]);

    float x = 2 * sin(time * 0.25) + x1;
    float y = y1;
    float z = 2 * cos(time * 0.25) + z1;

    translationMatrixCopy.translate(x, y, z);

    rotationMatrixCopy.rotate(time * model->rotation[0], 1, 0, 0);
    rotationMatrixCopy.rotate(time * model->rotation[1], 0, 1, 0);
    rotationMatrixCopy.rotate(time * model->rotation[2], 0, 0, 1);

    return translationMatrixCopy * model->scaleMatrix * rotationMatrixCopy;
}

// Transformation Function: Incoming Rock
QMatrix4x4 transform_IncomingRock(float time, void *modelObject) {
    ModelObject *model = (ModelObject *)modelObject;
    QMatrix4x4 translationMatrixCopy = model->translationMatrix;
    QMatrix4x4 rotationMatrixCopy = model->rotationMatrix;

    time = fmod(time, 20);

    float x = time * model->translation[0];
    float y = time * model->translation[1];
    float z = time * model->translation[2];

    translationMatrixCopy.translate(x, y, z);

    rotationMatrixCopy.rotate(time * model->rotation[0], 1, 0, 0);
    rotationMatrixCopy.rotate(time * model->rotation[1], 0, 1, 0);
    rotationMatrixCopy.rotate(time * model->rotation[2], 0, 0, 1);

    return translationMatrixCopy * model->scaleMatrix * rotationMatrixCopy;
}
