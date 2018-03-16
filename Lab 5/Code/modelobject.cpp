#include "modelobject.h"


ModelObject::ModelObject()
{}

// Function for setting object transforms.
void ModelObject::setTransform(QMatrix4x4 translationMatrix, QMatrix4x4 rotationMatrix, QMatrix4x4 scaleMatrix) {
    this->translationMatrix = translationMatrix;
    this->rotationMatrix = rotationMatrix;
    this->scaleMatrix = scaleMatrix;
}

// Function for setting animation settings.
void ModelObject::setAnimationFactors (float rotation, float translation) {
    this->rotation = rotation;
    this->translation = translation;
}
