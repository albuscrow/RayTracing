//
// Created by ac on 15-4-26.
//

#include "RayTracer.h"
#include "constant.h"

namespace acRayTracing {
    const PrimitiveSharePointer RayTracer::tracing(acRayTracing::Ray ray, int Depth, float incidentRefractiveIndex,
                                                   acRayTracing::Color &accumulate, float &distance) {
        if (Depth > TRACEDEPTH) {
            return nullptr;
        }

        PrimitiveSharePointer primitive;
        int result = scene.getNearestPrimitive(ray, distance, primitive);

        if (result == MISS) {
            return nullptr;
        }

        if (primitive->isLightP()) {
            accumulate = Color(1, 1, 1);
            return primitive;
        }

        getColorFromLight(ray, distance, primitive, accumulate);

        //reflect

        //refrect

        return primitive;

    }

    void RayTracer::getColorFromLight(Ray const &ray, float const &distance, PrimitiveSharePointer const &primitive,
                                      Color &accumulate) {

        Vector3 const &viewDirection = ray.getDirection();
        Vector3 const hitPoint = ray.getStartPoint() + viewDirection * distance;
        for (LightSharePointer &light : scene.getLights()) {

            Vector3 lightDirection = light->getCentre() - hitPoint;
            float distanceFromLight = lightDirection.length();
            float shadeIndex = calShade(light, distanceFromLight, hitPoint, lightDirection);
            if (shadeIndex > 0) {
                lightDirection /= distanceFromLight;
                Vector3 const normal = primitive->getNormal(hitPoint);
                if (primitive->getDiffuse() > 0) {
                    float dot = normal.dot(lightDirection);
                    if (dot > 0) {
                        accumulate += dot * primitive->getDiffuse() * shadeIndex
                                      * primitive->getColor()
                                      * light->getColor();
                    }
                }

                if (primitive->getSpecular() > 0) {
                    Vector3 reflectDirection = lightDirection -
                                               2.0f * lightDirection.dot(normal) * normal;
                    float dot = reflectDirection.dot(viewDirection);
                    if (dot > 0) {
                        accumulate += powf(dot, primitive->getShinness())
                                      * primitive->getSpecular() * shadeIndex
                                      * light->getColor();
                    }
                }
            }
        }
    }

    float RayTracer::calShade(LightSharePointer &light, float distanceFromLight, Vector3 const hitPosition,
                              Vector3 LightDirection) {
        return 0;
    }
}
