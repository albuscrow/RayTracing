//
// Created by ac on 15-4-26.
//

#include "Scene.h"
#include <limits>
#include "constant.h"
#include "RayTracer.h"

void acRayTracing::Scene::readObj(string fileName) {

}

int acRayTracing::Scene::getNearestPrimitive(acRayTracing::Ray ray, float &distance,
                                             acRayTracing::PrimitiveSharePointer &primitivePointers) {
    distance = std::numeric_limits<float>::max();
    float auxDistance;
    int finalResult = MISS, auxRes;
    for (auto &primitive : primitives) {
        auxRes = primitive->intersect(ray, auxDistance);
        if (auxRes != MISS && auxDistance < distance) {
            distance = auxDistance;
            finalResult = auxRes;
        }
    }
    return finalResult;
}

acRayTracing::Vector3 const acRayTracing::Primitive::getNormal(acRayTracing::Position position) {
    return Vector3();
}

acRayTracing::LightPointers acRayTracing::Scene::getLights() {
    return lights;
}

int acRayTracing::Primitive::intersect(acRayTracing::Ray ray, float &distance) {
    return 0;
}

const acRayTracing::Position acRayTracing::Light::getCentre() {
    return acRayTracing::Vector3();
}
