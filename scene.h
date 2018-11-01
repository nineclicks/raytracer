#ifndef _SCENE_H_
#define _SCENE_H_

#include <limits>
#include <cstdlib>
#include <vector>
#include "object.h"
#include "vector.h"
#include "fimage.h"
#include <time.h>

#define RAND (double)rand()/RAND_MAX
#define INF std::numeric_limits<double>::infinity()
#define PI 3.14159265359;


using namespace std;

class Object;
class Light;
class hit;

class Scene {
public:
    
    vector<Object*> Objects;
    vector<Light*> Lights;
    vector<fimage*> Textures;
    
    fimage image;
    
    char output[64];

    double windowScale;
    double scale; 
    double superSample;
    double shadowSample;
    int recursion;
    int sampleMethod;
    int softShadows;
    int occlusion;
    int occlusionSamples = 25;
    double occlusionRadius = 1.0;
    Vec win; // window reletive to pov
    Vec pov; // pov
    Vec p;   // aboslute center point of window
    Vec u, v;
    
    Vec ambient;    
    
    Scene();
    
    fimage *AddTexture(const char *c);
    
    void setRes(int w, int h, int d, int div);
    
    void setCamera(Vec origin, Vec focus, double s);
    
    Scene(int w, int h, int d);
    
    void windowVect();
    
    Vec getPixelCoordinate(double x, double y);
    
    void getPixelVector(double x, double y, Vec *Rp, Vec *RA);
    
    Light* newLight();
    
    template<typename T>
    T *newObject() {
        T *p = new T;
        Objects.push_back(p);
        return p; 
    }
    
    void drawScene();
    double SampleLight(hit*, Vec*, Light*, int);
    double Occlusion(hit h, Vec d);
    Vec TraceLight(hit, Vec);
    hit Closest(Vec, Vec);
    Vec Cast(Vec o, Vec d, int depth);
    
};



#endif
