
#include "object.h"

Texture::Texture() {
    pattern = Texture::NONE;
    intensity = 0.0;
    reflection = 0.0;
    
}

void Object::Speak(){printf("Object\n");}
hit Object::Trace(Vec, Vec) {
    return hit(0);
}
void Object::SetNormal(Vec) {}
Vec Object::GetNormal(){ return Vec(0.0,0.0,0.0); }
void Object::SetNormal(double, double, double) {}
void Object::Setu(Vec) {}
void Object::SetRadius(double) {}
void Object::SetIntensity(double) {}
void Object::AddVert(double, double, double) {}
void Object::AddVert(Vec) {}
void Object::SetVert(int, double, double, double) {}
void Object::SetVert(Vec, Vec, Vec) {}
Vec Object::Color(hit) { return Vec(0.0,0.0,0.0); }

void Light::SetIntensity(double i) {
    intensity = i;
}

void Sphere::SetIntensity(double i) {
    texture.intensity = i;
}

void Plane::SetIntensity(double i) {
    texture.intensity = i;
}

void Triangle::SetIntensity(double i) {
    texture.intensity = i;
}

void Poly::SetIntensity(double i) {
    texture.intensity = i;
}

void Light::SetRadius(double r) { radius = r; }

Light::Light() {
    radius = 0.0;
}

Vec Light::RandomSpot() {
    Vec v = origin;
    v[0] += (RAND * radius * 2.0) - (radius);
    v[1] += (RAND * radius * 2.0) - (radius);
    v[2] += (RAND * radius * 2.0) - (radius);
    return v;
}

void Plane::Speak(){printf("Plane\n");}

hit Plane::Trace(Vec o, Vec d) {
    Vec location;
    d.normalize();
    Vec n = normal.GetNormalized();
    double dDotNormal = d.dot(n);
    if (dDotNormal == 0.0)
        return hit(0);
    double t = -1.0 * (o - origin).dot(n) / dDotNormal;
    if (t < 0.0) {
        return hit(0);
    }
    
    location = o + (d * t);
    
    return hit(this, location, t, n);
}

Vec Plane::Color(hit t) {
    Vec location = t.location;
    Vec color = texture.color;
    if (texture.pattern == Texture::CHECKER) {
        int col = 0;
        for (int i = 0; i <3; i++){
            if ((int)((location[i] + 0.001) / texture.scale) % 2)
                col ^= 1;
            if (location[i] < 0.001)
                col ^= 1;
        }
        if (!col)
            color = texture.color2;
    }
    return color;
}

void Plane::SetNormal(Vec v) { normal = v; }
void Plane::SetNormal(double x, double y, double z) { normal.set(x,y,z); }
Vec Plane::GetNormal() { return normal; }
void Plane::Setu(Vec v) { u = v; }
Vec Plane::Getu() { return u; }


Object::Geometry Triangle::type(){return TRIANGLE;}
void Triangle::Speak(){printf("Triangle\n");}

void Triangle::AddVert(double x, double y, double z) {
    vert[2] = vert[1];
    vert[1] = vert[0];
    vert[0] = Vec(x,y,z);
    CalcNorm();
}

void Triangle::SetVert(int i, double x, double y, double z) {
    vert[i] = Vec(x,y,z);
    CalcNorm();
}

void Triangle::SetVert(Vec x, Vec y, Vec z) {
    vert[0] = x;
    vert[1] = y;
    vert[2] = z;
    CalcNorm();
}


void Triangle::AddVert(Vec v) {
    vert[2] = vert[1];
    vert[1] = vert[0];
    vert[0] = v;
    CalcNorm();
}

void Triangle::CalcNorm() {
    normal = (vert[0] - vert[1]).cross(vert[0] - vert[2]);
    origin = vert[0];
}

hit Triangle::Trace(Vec o, Vec d) {
    Vec location;
    d.normalize();
    Vec n = normal.GetNormalized();
    double dDotNormal = d.dot(n);
    if (dDotNormal == 0.0)
        return hit(0);
    //if (dDotNormal < 0.0)
    //    n = n * -1.0;
    double t = -1.0 * (o - origin).dot(n) / dDotNormal;
    if (t < 0.0) {
        return hit(0);
    }
    location = o + (d * t);
    Vec b = Barycentric(location);
    if (Inside(b))
        return hit(this, location, t, n, b);
    else
        return hit(0);
}

bool Triangle::Inside(Vec v) {
    return (v[0] >= 0.0 && v[1] >= 0.0 && v[2] >= 0);
}

Vec Triangle::Barycentric(Vec location) {
    double ABC = normal.dot((vert[1] - vert[0]).cross(vert[2] - vert[0]));
    double PBC = normal.dot((vert[1] - location).cross(vert[2] - location));
    double PCA = normal.dot((vert[2] - location).cross(vert[0] - location));
    double u = PBC / ABC;
    double v = PCA / ABC;
    return Vec(u, v, 1.0 - u - v);
}

Vec Triangle::Color(hit t) {
    if (texture.pattern == Texture::IMAGE) {
        double u = t.barycentric[0];
        double v = t.barycentric[1];
        double w = t.barycentric[2];
        double x = u * texture.imageVert[0][0] + v * texture.imageVert[1][0] + w * texture.imageVert[2][0];
        double y = u * texture.imageVert[0][1] + v * texture.imageVert[1][1] + w * texture.imageVert[2][1];
        x = x * (double)texture.image->width;
        y = y * (double)texture.image->height;
        return texture.image->getPixel(x,y);
    } else {
        return texture.color;
    }
}

void Poly::Speak(){printf("Poly\n");}


void Sphere::Speak(){printf("Sphere\n");}

void Sphere::SetRadius(double r) { radius = r; }

extern int superX;

hit Sphere::Trace(Vec o, Vec d) {
    d.normalize();
    double a = d.dot(d);
    double b = (d.dot(o - origin));
    double c = (o - origin).dot(o - origin) - radius * radius;
    double s = b*b - a * c;
    if (s < 0.0)
        return hit(0);
    double d1 = (-1.0 * b + sqrt(s)) / a;
    double d2 = (-1.0 * b - sqrt(s)) / a;

    double l = 1.0 / 80.0 * (float)superX;
    double h = .225;
    double z = .25;
    double cmin, cmax;
    
    if (d2 < d1 && d2 > 0.0) {
        Vec location = (d * d2) + o;
        
        Vec n = location - origin;
        n.normalize();
        
        hit t = hit(this, location, d2, n);

        Vec n2 = t.normal;

        double x = n2.AngleX();
        double y = n2.AngleY();

        x = fmod(x,z);

        if (this->texture.intensity == 1.0)
          return t;
        
        cmin = l - (h / 2.0);
        cmax = l + (h / 2.0);
        while (cmin < 0.0)
          cmin += z;
        while (cmax >= z)
          cmax -= z;
        if ((x > cmin && x < cmax) || (cmin > cmax && (x > cmin || x < cmax)))
          return t;

    }

    if (d1 < 0.0)
        return hit(0);
    Vec location = (d * d1) + o;
    
    Vec n = location - origin;
    n.normalize();
    
    hit t = hit(this, location, d1, n);

    Vec n2 = t.normal;

    double x = n2.AngleX();
    double y = n2.AngleY();

    if (this->texture.intensity == 1.0)
      return t;

    x = fmod(x,z);

    cmin = l - (h / 2.0);
    cmax = l + (h / 2.0);
    while (cmin < 0.0)
      cmin += z;
    while (cmax >= z)
      cmax -= z;
    if ((x > cmin && x < cmax) || (cmin > cmax && (x > cmin || x < cmax)))
      return t;
    
    return hit(0);
}

Vec Sphere::Color(hit t) {
    Vec color = texture.color;
    Vec n = t.normal;
    
    if (texture.pattern == Texture::CHECKER) {
        int col = 0;
        double x = n.AngleX();
        double y = n.AngleY();
        
        if ((int)((x + 0.001) / texture.scale) % 2)
            col ^= 1;
        if ((int)((y + 0.001) / texture.scale) % 2)
            col ^= 1;
        
        
        if (!col)
            color = texture.color2;
    } else if (texture.pattern == Texture::IMAGE) {
        
        double x = n.AngleX();
        double y = n.AngleY();
        
        if(texture.image != NULL) {
            x = x * (double)texture.image->width / 2.0;
            y = (double)texture.image->height - y * (double)texture.image->height;
            return texture.image->getPixel(x,y);
        }
    }
    return color;
}
