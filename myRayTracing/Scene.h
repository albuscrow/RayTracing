//
// Created by ac on 15-4-26.
//

#ifndef AC_RAY_TRACING_SCENE_H
#define AC_RAY_TRACING_SCENE_H

#include <string>
#include <vector>
#include <memory>
#include "common.h"


using std::string;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;
namespace acRayTracing {

    class Ray;
    class Material {

    private:
    public:
        float getDiffuse() const {
            return diffuse;
        }

        float getSpecular() const {
            return specular;
        }

        Color const &getColor() const {
            return color;
        }

        float getShinness() const {
            return shinness;
        }

    private:
        float diffuse;
        float specular;

    private:
        float shinness;

    private:
        Color color;

    };

    class Primitive {
    public:
        int intersect(Ray ray, float &distance);

        bool isLightP(){
            return isLight;
        }

        Vector3 const getNormal(Position position);

        virtual Position const getCentre() = 0;

        float getDiffuse() {
            return material.getDiffuse();
        }

        float getSpecular() {
            return material.getSpecular();
        }

        Color getColor(){
            return material.getColor();
        }

        float getShinness() {
            return material.getShinness();
        }

    private:
        bool isLight;
        Material material;
    };

    typedef shared_ptr<Primitive> PrimitiveSharePointer;


    class Sphere : Primitive {

    };

    class Plane : Primitive {

    };

    class Triangle : Primitive {

    };


    class Light : public Primitive {
    public:
        virtual Position const getCentre() override;
    };
    typedef shared_ptr<Light> LightSharePointer;

    typedef vector<shared_ptr<Primitive>> PrimitivePointers;
    typedef vector<shared_ptr<Light>> LightPointers;

    class Scene {
    public :
        void readObj(string fileName);

        int getNearestPrimitive(Ray, float &distance, PrimitiveSharePointer &primitivePointers);

        LightPointers getLights();

    private:
        PrimitivePointers primitives;
        LightPointers lights;

    };
}

#endif //AC_RAY_TRACING_SCENE_H
