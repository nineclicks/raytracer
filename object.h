#ifndef _OBJECT_H_
#define _OBJECT_H_
#include <cmath>
#include "vector.h"
#include "fimage.h"
#include "scene.h"
#include "defines.h"
extern unsigned int *state;

//class Scene;
struct hit;

class Texture {
public:
    Texture();
    enum Pattern{NONE,CHECKER,IMAGE};
    Vec color;
    Vec color2;
    Vec color3;
    double reflection;
    double opacity;
    double refraction;
    Pattern pattern;
    double scale;
    fimage* image;
    double imageVert[3][2];
    double intensity;
};

class Object {
public:
    enum Geometry{NONE,PLANE,TRIANGLE,POLY,SPHERE};
    Texture texture;  
    Vec origin; 
    virtual void Speak();
    virtual hit Trace(Vec, Vec);
    virtual void SetNormal(Vec);
    virtual Vec GetNormal();
    virtual void SetNormal(double, double, double);
    virtual void Setu(Vec);
    virtual void SetRadius(double);
    virtual void SetIntensity(double);
    virtual void AddVert(double, double, double);
    virtual void AddVert(Vec);
    virtual void SetVert(int, double, double, double);
    virtual void SetVert(Vec, Vec, Vec);
    virtual Vec Color(hit);
};

class Plane : public Object {
    
    Vec normal;
    Vec u; // rotation
public:
    hit Trace(Vec o, Vec d);
    
    void SetNormal(Vec v);
    void SetNormal(double x, double y, double z);
    Vec GetNormal();
    void Setu(Vec v);
    Vec Getu();
    Vec Color(hit);
    void Speak();
    void SetIntensity(double);
};

class Triangle : public Object {
public:
    Vec vert[3];
    Vec normal;
    Geometry type();
    void Speak();
    void AddVert(double, double, double);
    void AddVert(Vec);
    void SetVert(int, double, double, double);
    void SetVert(Vec x, Vec y, Vec z);
    void CalcNorm();
    
    hit Trace(Vec o, Vec d);
    Vec Color(hit);
    
    bool Inside(Vec v);
    Vec Barycentric(Vec);
    void SetIntensity(double);
};

class Poly : public Object {
public:
    Vec *vertices;
    double nVertices;
    Vec normal;
    void Speak();
    void SetIntensity(double);
};

class Sphere : public Object {
public:
    double radius;
    Vec u; //rotation
    Vec v; //rotation
    
    void SetRadius(double r);
    
    hit Trace(Vec o, Vec d);
    Vec Color(hit);
    void Speak();
    void SetIntensity(double);
};

class Light : public Object {
public:
    double intensity;
    void SetIntensity(double);
    void SetRadius(double r);
    Vec RandomSpot();
    double radius;
    Light();
};

struct hit {
    Vec location;
    Vec normal;
    double distance;
    bool contact;
    Object *object;
    Vec barycentric;
    hit(Object * o, Vec l, double d, Vec n) {
        object = o;
        location = l;
        normal = n;
        distance = d;
        contact = 1;
    }
    hit(Object * o, Vec l, double d, Vec n, Vec b) {
        barycentric = b;
        object = o;
        location = l;
        normal = n;
        distance = d;
        contact = 1;
    }
    hit(bool c) {
        contact = c;
        object = NULL;
    }
};

#endif
