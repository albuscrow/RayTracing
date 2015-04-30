//
// Created by ac on 15-4-26.
//

#ifndef AC_RAY_TRACING_RAYTRACER_H
#define AC_RAY_TRACING_RAYTRACER_H

#include <common.h>
#include <vector>
#include <memory>
#include <QObject>
#include "Scene.h"

using std::vector;
using std::unique_ptr;
using std::shared_ptr;

namespace acRayTracing {

    class Primitive;

    typedef shared_ptr<Primitive> PrimitiveSharePointer;

    class Ray {
    public:
        Ray(Position startPoint, Vector3 direction)
                : startPoint(startPoint), direction(direction) { }


        Position const &getStartPoint() const {
            return startPoint;
        }

        Vector3 const &getDirection() const {
            return direction;
        }

    private:
        Vector3 direction;
        Position startPoint;

    };

    class RayTracer : public QObject {
    Q_OBJECT
    public:
        RayTracer()
                : eyePosition(0, 0, 1), screenWidth(400), screenHeight(300) { }

        void setEyeInf(const Position &eyePosition) {
            this->eyePosition = eyePosition;
        }

        void setScreen(const int &width, const int &height) {
            this->screenHeight = height;
            this->screenWidth = width;
        }

        Ray getRay(int XPixel, int YPixel) {
            float windowHeight = 1.2f;
            float windowWidth = windowHeight / screenHeight * screenWidth;
            float delta = windowHeight / screenHeight;
            Position endPoint(eyePosition.x - windowWidth / 2 + XPixel * delta,
                              eyePosition.y + windowHeight / 2 - YPixel * delta, eyePosition.z - 1);
            return Ray(eyePosition, endPoint - eyePosition);
        }

        const PrimitiveSharePointer tracing(Ray, int Depth, float incidentRefractiveIndex, Color &accumulate,
                                            float &distance);

    private:

        //这边假设视窗在 z=eyePosition.z-1 平面上。并且眼睛朝向Z轴负方向。张角为60度
        Position eyePosition;
        int screenWidth, screenHeight;
        Scene scene;


    private:
        void getColorFromLight(Ray const &ray, float const &distance, PrimitiveSharePointer const &primitive,
                               Color &accumulate);


        float calShade(LightSharePointer &light, float distanceFromLight, Vector3 const hitPosition,
                       Vector3 LightDirection);

    signals:

        void updateScreenSignal(int width, int height, Color color);

    };

}

#endif //AC_RAY_TRACING_RAYTRACER_H
