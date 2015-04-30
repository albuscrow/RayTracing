//
// Created by ac on 15-4-25.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#ifndef RAY_TRACING_COMMON_H
#define RAY_TRACING_COMMON_H

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#include <cmath>


namespace acRayTracing {

    class Vector3 {
    public:
        Vector3(float x, float y, float z)
                : x(x), y(y), z(z){
        };

        Vector3(const Vector3&) = default;
        Vector3() = default;

        Vector3& normalize(){
            float aux = 1 / length();
            x *= aux;
            y *= aux;
            z *= aux;
        }


        Vector3 operator+(Vector3 v) const {
            return Vector3(this->x + v.x, this->y + v.y, this->z + v.z);
        }


        Vector3 operator-(Vector3 v) const {
            return Vector3(this->x - v.x, this->y - v.y, this->z - v.z);
        }

        Vector3 operator*(Vector3 v) {
            return Vector3(this->x * v.x, this->y * v.y, this->z * v.z);
        }


        Vector3 operator*(float f) const {
            return Vector3(this->x * f, this->y * f, this->z * f);
        }

        friend Vector3 operator*(float f, const Vector3 &v) {
            return Vector3(v.x * f, v.y * f, v.z * f);
        }

        Vector3 operator/(Vector3 v) {
            return Vector3(this->x / v.x, this->y / v.y, this->z / v.z);
        }

        void operator+=(Vector3 v) {
            this->x += v.x;
            this->y += v.y;
            this->z += v.z;
        }

        void operator-=(Vector3 v) {
            this->x -= v.x;
            this->y -= v.y;
            this->z -= v.z;
        }

        void operator*=(Vector3 v) {
            this->x *= v.x;
            this->y *= v.y;
            this->z *= v.z;
        }


        void operator*=(float f) {
            this->x *= f;
            this->y *= f;
            this->z *= f;
        }

        void operator/=(float f) {
            this->x /= f;
            this->y /= f;
            this->z /= f;
        }

        float x = 0;
        float y = 0;
        float z = 0;

        float length(){
            return (float) sqrt(x * x + y * y + z * z);
        }

        float dot(Vector3 v) const{
            return x * v.x + y * v.y + z * v.z;
        }
    };

    typedef Vector3 Position;
    typedef Vector3 Color;

}


#endif //RAY_TRACING_COMMON_H

#pragma clang diagnostic pop