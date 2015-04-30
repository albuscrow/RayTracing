// -----------------------------------------------------------
// raytracer.cpp
// 2004 - Jacco Bikker - jacco@bik5.com - www.bik5.com -   <><
// -----------------------------------------------------------

#include <string.h>
#include "raytracer.h"
#include "scene.h"
#include <iostream>
#include <strings.h>

using Raytracer::vector3;

namespace Raytracer {

    Ray::Ray(const vector3 &a_Origin, const vector3 &a_Dir) :
            m_Origin(a_Origin),
            m_Direction(a_Dir) {
    }

    Engine::Engine() {
        m_Scene = new Scene();
    }

    Engine::~Engine() {
        delete m_Scene;
    }

// -----------------------------------------------------------
// Engine::SetTarget
// Sets the render target canvas
// -----------------------------------------------------------
    void Engine::SetTarget(Pixel *a_Dest, int a_Width, int a_Height) {
        // set pixel buffer address & size
        m_Dest = a_Dest;
        m_Width = a_Width;
        m_Height = a_Height;
    }

// -----------------------------------------------------------
// Engine::Raytrace
// Naive ray tracing: Intersects the ray with every primitive
// in the scene to determine the closest intersection
// -----------------------------------------------------------
    Primitive *Engine::Raytrace(Ray a_Ray, Color &a_Acc, int a_Depth, float a_RIndex, float &a_Dist) {
        if (a_Depth > TRACEDEPTH) {
            return 0;
        }
        // trace primary ray
        a_Dist = 1000000.0f;

        //point of intersection
        vector3 pi;

        Primitive *prim = nullptr;
        int result;
        // find the nearest intersection
        for (int s = 0; s < m_Scene->GetNrPrimitives(); ++s) {
            Primitive *pr = m_Scene->GetPrimitive(s);
            int res;
            if (res = pr->Intersect(a_Ray, a_Dist)) {
                prim = pr;
                result = res; // 0 = miss, 1 = hit, -1 = hit from inside primitive
            }
        }

        // no hit, terminate ray
        if (!prim) {
            return 0;
        }

        // handle intersection
        if (prim->IsLight()) {
            // we hit a light, stop tracing
            a_Acc = Color(1, 1, 1);
        } else {

            // determine color at point of intersection

            vector3 const &V = a_Ray.GetDirection();
            pi = a_Ray.GetOrigin() + V * a_Dist;
            vector3 N = prim->GetNormal(pi);

            // trace lights
            for (int l = 0; l < m_Scene->GetNrPrimitives(); l++) {
                Primitive *p = m_Scene->GetPrimitive(l);
                if (p->IsLight()) {
                    Primitive *light = p;

                    vector3 L = ((Sphere *) light)->GetCentre() - pi;
                    float tdist = LENGTH(L);
                    L *= (1.0f / tdist);

                    float shade = calShade(light, tdist, pi, L);
                    if (shade > 0) {

                        // calculate diffuse shading
                        if (prim->getMaterial()->GetDiffuse() > 0) {
                            float dot = DOT(N, L);
                            if (dot > 0) {
                                float diff = dot * prim->getMaterial()->GetDiffuse() * shade;
                                // add diffuse component to ray color
                                a_Acc += diff * prim->getMaterial()->GetColor() * light->getMaterial()->GetColor();
                            }
                        }

                        if (prim->getMaterial()->GetSpecular() > 0) {
                            vector3 R = L - 2.0f * DOT(L, N) * N;
                            float dot = DOT(R, V);
                            if (dot > 0) {
                                float spec = powf(dot, prim->getMaterial()->getShiny()) *
                                             prim->getMaterial()->GetSpecular() * shade;
                                a_Acc += spec * light->getMaterial()->GetColor();
                            }
                        }
                    }
                }
            }

            //reflect
            float refl = prim->getMaterial()->GetReflection();
            if (refl > 0.0f) {

                float drefl = prim->getMaterial()->getDiffuseRefl();
                if ((drefl > 0) && (a_Depth < 2)) {
                    // calculate diffuse reflection
                    vector3 N = prim->GetNormal(pi);
                    vector3 RP = a_Ray.GetDirection() - 2.0f * DOT(a_Ray.GetDirection(), N) * N;
                    vector3 RN1 = vector3(RP.z, RP.y, -RP.x);
                    vector3 RN2 = RP.Cross(RN1);
                    refl *= 1 / 16;
                    for (int i = 0; i < 16; i++) {
                        float xoffs, yoffs;
                        do {
                            xoffs = m_Twister.Rand() * drefl;
                            yoffs = m_Twister.Rand() * drefl;
                        }
                        while ((xoffs * xoffs + yoffs * yoffs) > (drefl * drefl));
                        vector3 R = RP + RN1 * xoffs + RN2 * yoffs * drefl;
                        NORMALIZE(R);
                        float dist;
                        Color rcol(0, 0, 0);
                        Raytrace(Ray(pi + R * EPSILON, R), rcol, a_Depth + 1, a_RIndex, dist);
                        a_Acc += refl * rcol * prim->getMaterial()->GetColor();
                    }
                } else {

                    Color rcol(0, 0, 0);
                    vector3 R = V - 2.0f * DOT(V, N) * N;
                    vector3 origin = pi + R * EPSILON;
                    Ray ray = Ray(origin, R);
                    float dist;
                    Raytrace(ray, rcol, a_Depth + 1, a_RIndex, dist);
                    a_Acc += refl * rcol * prim->getMaterial()->GetColor();
                }
            }

            float refr = prim->getMaterial()->getRefraction();
            float rindex = prim->getMaterial()->getRefrIndex();
            if (refr > 0.0f && rindex > 0.0f) {
                float n = a_RIndex / rindex;
                N = N * result;
                float cosI = -DOT(N, V);
                float cosT2 = 1.0f - n * n * (1 - cosI * cosI);
                if (cosT2 > 0.0f) {
                    vector3 T = (n * V) + (n * cosI - sqrt(cosT2)) * N;
                    Color rcol;
                    vector3 d = pi + T * EPSILON;
                    Ray ray = Ray(d, T);
                    float dist;
                    Raytrace(ray, rcol, a_Depth + 1, rindex, dist);
                    Color absorbance = prim->getMaterial()->GetColor() * 0.15f * -dist;
                    Color transparency = Color(expf(absorbance.r),
                                               expf(absorbance.g),
                                               expf(absorbance.b));
                    a_Acc += rcol * transparency;
                }
            }
        }
        // return pointer to primitive hit by primary ray
        return prim;
    }

    float Engine::calShade(Primitive *light, const float tdist, const vector3 &pi, vector3 &L) {
        float shade = 1.0f;
        if (light->GetType() == Primitive::SPHERE) {
            vector3 d = pi + L * EPSILON;
            Ray r(d, L);
            Primitive *prim = findNearest(light, r, tdist);
            if (prim != light) {
                shade = 0;
            }
        } else if (light->GetType() == Primitive::AABB) {
            shade = 0;
            Box *b = (Box *) light;
            L = b->GetPos() + 0.5f * b->GetSize() - pi;
            NORMALIZE(L);

            float deltaX = b->GetSize().x / 4;
            float deltaZ = b->GetSize().z / 4;

            for (int x = 0; x < 4; ++x) {
                for (int z = 0; z < 4; ++z) {
                    vector3 lp(b->GetPos().x + (x + m_Twister.myRand()) * deltaX, b->GetPos().y,
                               b->GetPos().z + (z + m_Twister.myRand()) * deltaZ);
                    vector3 dir = lp - pi;
                    float ldist = LENGTH(dir);
                    dir *= 1.0f / ldist;
                    vector3 origin = pi + dir * EPSILON;
                    Primitive *prim = findNearest(light, Ray(origin, dir), tdist);
                    if (prim == light) {
                        shade += 1.0f / 16;
                    }
                }
            }
        }
        return shade;
    }


// -----------------------------------------------------------
// Engine::InitRender
// Initializes the renderer, by resetting the line / tile
// counters and precalculating some values
// -----------------------------------------------------------
    void Engine::InitRender() {
        // set firts line to draw to
        m_CurrLine = 0;
        // set pixel buffer address of first pixel
        m_PPos = 0 * m_Width;
        // screen plane in world space coordinates
        m_WX1 = -4, m_WX2 = 4, m_WY1 = m_SY = 3, m_WY2 = -3;
        // calculate deltas for interpolation
        m_DX = (m_WX2 - m_WX1) / m_Width;
        m_DY = (m_WY2 - m_WY1) / m_Height;
        m_SY += 20 * m_DY;
        // allocate space to store pointers to primitives for previous line
        m_LastRow = new Primitive *[m_Width];
        memset(m_LastRow, 0, m_Width * 4);
    }


// -----------------------------------------------------------
// Engine::Render
// Fires rays in the scene one scanline at a time, from left
// to right
// -----------------------------------------------------------
    bool Engine::Render() {
        // render scene
        vector3 o(0, 0, -5);
        // initialize timer
        int msecs = GetTickCount();
        // reset last found primitive pointer
        Primitive *lastprim = 0;
        // render remaining lines
        for (int y = m_CurrLine; y < m_Height; y++) {
            m_SX = m_WX1;
            // render pixels for current line
            for (int x = 0; x < m_Width; x++) {
                // fire primary ray
                Color acc(0, 0, 0);
                vector3 dir = vector3(m_SX, m_SY, 0) - o;
                NORMALIZE(dir);
                Ray r(o, dir);
                float dist;
                Primitive *prim = Raytrace(r, acc, 1, 1.0f, dist);
                int red;
                int green;
                int blue;
                if (prim == lastprim) {
                    red = (int) (acc.r * 256);
                    green = (int) (acc.g * 256);
                    blue = (int) (acc.b * 256);
                } else {
                    lastprim = prim;
                    Color acc(0, 0, 0);
                    for (int tx = -1; tx < 2; tx++) {
                        for (int ty = -1; ty < 2; ty++) {
                            vector3 dir = vector3(m_SX + m_DX * tx / 2.0f, m_SY + m_DY * ty / 2.0f, 0) - o;
                            NORMALIZE(dir);
                            Ray r(o, dir);
                            float dist;
                            Primitive *prim = Raytrace(r, acc, 1, 1.0f, dist);
                        }
                    }
                    red = (int) (acc.r * (256.0f / 9));
                    green = (int) (acc.g * (256.0f / 9));
                    blue = (int) (acc.b * (256.0f / 9));
                }
                if (red > 255) {
                    red = 255;
                }

                if (green > 255) {
                    green = 255;
                }

                if (blue > 255) {
                    blue = 255;
                }

                m_Dest[m_PPos++] = (red << 16) + (green << 8) + blue;
                m_SX += m_DX;
            }
            m_SY += m_DY;
        }
        emit updateScreenSignal(m_Dest);
        // all done
        return true;
    }

    Primitive *Engine::findNearest(Primitive *light, Ray r, float tdist) {
        for (int s = 0; s < m_Scene->GetNrPrimitives(); ++s) {
            Primitive *prim = m_Scene->GetPrimitive(s);
            if ((prim != light)
                && (prim->Intersect(r, tdist))) {
                return prim;
            }
        }
        return light;
    }
}; // namespace Raytracer