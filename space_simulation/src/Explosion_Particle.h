#pragma once
#ifndef _EXPLOSION_PARTICLE_H_
#define _EXPLOSION_PARTICLE_H_

#define _USE_MATH_DEFINES
#include <memory>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#define EIGEN_DONT_ALIGN_STATICALLY
#include <Eigen/Dense>

class MatrixStack;
class Program;
class Texture;

class Explosion_Particle
{
public:
    
    Explosion_Particle(int index);
    virtual ~Explosion_Particle();
    void rebirth(float t, const bool *keyToggles, bool starting);
    void step(float t, float h, const Eigen::Vector3f &g, const bool *keyToggles, bool keepSpawning);
    
    // Static, shared by all particles
    static void init(int n);
    static void draw(const std::vector< std::shared_ptr<Explosion_Particle> > &particles,
                     std::shared_ptr<Program> prog);
    static float randFloat(float l, float h);
    
private:
    // Properties that are fixed
    Eigen::Map<Eigen::Vector3f> color; // color (mapped to a location in colBuf)
    float &scale;                      // size (mapped to a location in scaBuf)
    
    // Properties that changes every rebirth
    float m;        // mass
    // float d;        // viscous damping
    float lifespan; // how long this particle lives
    float tEnd;     // time this particle dies
    
    // Properties that changes every frame
    Eigen::Map<Eigen::Vector3f> x; // position (mapped to a location in posBuf)
    Eigen::Vector3f v;             // velocity
    float &alpha;                  // mapped to a location in alpBuf
    
    // Static, shared by all particles
    static std::vector<float> posBuf;
    static std::vector<float> colBuf;
    static std::vector<float> alpBuf;
    static std::vector<float> scaBuf;
    static GLuint posBufID;
    static GLuint colBufID;
    static GLuint alpBufID;
    static GLuint scaBufID;
    
    // added by me
    int index;
    Eigen::Vector3f dir;
};

#endif

