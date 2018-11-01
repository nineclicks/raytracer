#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <cstdio>
#include <cmath>

class Vec {
private:
    double x[3];
public:
    double AngleX();
    double AngleY();
    double& operator[] (int i);
    Vec();
    
    Vec(double a, double b, double c);
    
    void set(double a, double b, double c);
    
    double magnitude();
    
    void normalize();
    
    Vec GetNormalized();
    
    Vec cross(Vec v);
    Vec blend(Vec, double);
    
    double dot(Vec v);
    
    Vec(const Vec &v);
    
    Vec& operator=(const Vec &v);
    
    Vec operator+(const Vec &v);
    
    Vec operator-(const Vec &v);
    Vec operator*(const double &f);
    Vec operator/(const double &f);
    Vec operator*=(const double &f);
    
    void print();
};

#endif
