#include "Laser_Particle.h"

#include <iostream>

#include "GLSL.h"
#include "MatrixStack.h"
#include "Program.h"
#include "Texture.h"

using namespace std;
using namespace Eigen;

vector<float> Laser_Particle::posBuf;
vector<float> Laser_Particle::colBuf;
vector<float> Laser_Particle::alpBuf;
vector<float> Laser_Particle::scaBuf;
GLuint Laser_Particle::posBufID;
GLuint Laser_Particle::colBufID;
GLuint Laser_Particle::alpBufID;
GLuint Laser_Particle::scaBufID;

// Before this constructor is called, posBuf must be a valid vector<float>.
// I.e., Particle::init(n) must be called first.
Laser_Particle::Laser_Particle(int index) :
    color(&colBuf[3*index]),
    scale(scaBuf[index]),
    x(&posBuf[3*index]),
    alpha(alpBuf[index])
{
    
    this->index = index;

    color = Vector3f(1.0f,1.0f,1.0f);
    scale = 0.05; // randFloat(0.1f, 0.2f);
    
    // Send color data to GPU
    glBindBuffer(GL_ARRAY_BUFFER, colBufID);
    glBufferSubData(GL_ARRAY_BUFFER, 3*index*sizeof(float), 3*sizeof(float), color.data());
    
    // Send scale data to GPU
    glBindBuffer(GL_ARRAY_BUFFER, scaBufID);
    glBufferSubData(GL_ARRAY_BUFFER, index*sizeof(float), sizeof(float), &scale);
}

Laser_Particle::~Laser_Particle()
{
}

void Laser_Particle::rebirth(float t, const bool *keyToggles)
{
    m = 1.0f;
    alpha = 0.2f;
    
    // m = randFloat(0.0f, m);
    // alpha = randFloat(0.0f, alpha);
    
    //========================================================================================
    //========================================================================================
    float r = 0.05;
    float a = randFloat(0, 2*M_PI);
    float c1 = randFloat(0,r*cos(a));
    float c2 = randFloat(0,r*sin(a));
    x << c1, c2, 0;
    v = Vector3f(0, 0, 0);
    //========================================================================================
    //========================================================================================

    lifespan = 1.0f;
    lifespan = randFloat(0.0f, lifespan);

    tEnd = t + lifespan;
}

void Laser_Particle::step(float t, float h, const Vector3f &g, const bool *keyToggles)
{
    if(t > tEnd) {
        rebirth(t, keyToggles);
    }
    // Update alpha based on current time
    alpha = (tEnd-t)/lifespan;
    
    //========================================================================================
    //========================================================================================
    // Accumulate forces
    Vector3f ft = Vector3f(0.0f, 0.0f, 5.0f);
    Vector3f f = ft;
    //========================================================================================
    //========================================================================================
    
    // Update velocity
    v += (h/m)*f;
    
    // Update position
    x += h*v;
    
    // Update color
    float r = x.norm();
    float i = 0.5;
    color.y() = 1.0f * ( (i + (0.8-i) * randFloat(0.0, 1.0)) * (1-r) );
    color.x() = 1.0f * ( (0.0 + (i-0.0) * randFloat(0.0, 1.0)) * (1-r) );
    
    // Send color data to GPU
    glBindBuffer(GL_ARRAY_BUFFER, colBufID);
    glBufferSubData(GL_ARRAY_BUFFER, 3*index*sizeof(float), 3*sizeof(float), color.data());

}

float Laser_Particle::randFloat(float l, float h)
{
    float r = rand() / (float)RAND_MAX;
    return (1.0f - r) * l + r * h;
}

void Laser_Particle::init(int n)
{
    posBuf.resize(3*n);
    colBuf.resize(3*n);
    alpBuf.resize(n);
    scaBuf.resize(n);
    
    for(int i = 0; i < n; ++i) {
        posBuf[3*i+0] = 0.0f;
        posBuf[3*i+1] = 0.0f;
        posBuf[3*i+2] = 0.0f;
        colBuf[3*i+0] = 1.0f;
        colBuf[3*i+1] = 1.0f;
        colBuf[3*i+2] = 1.0f;
        alpBuf[i] = 1.0f;
        scaBuf[i] = 1.0f;
    }

    // Generate buffer IDs
    GLuint bufs[4];
    glGenBuffers(4, bufs);
    posBufID = bufs[0];
    colBufID = bufs[1];
    alpBufID = bufs[2];
    scaBufID = bufs[3];
    
    // Send color buffer to GPU
    glBindBuffer(GL_ARRAY_BUFFER, colBufID);
    glBufferData(GL_ARRAY_BUFFER, colBuf.size()*sizeof(float), &colBuf[0], GL_STATIC_DRAW);
    
    // Send scale buffer to GPU
    glBindBuffer(GL_ARRAY_BUFFER, scaBufID);
    glBufferData(GL_ARRAY_BUFFER, scaBuf.size()*sizeof(float), &scaBuf[0], GL_STATIC_DRAW);
    
    assert(glGetError() == GL_NO_ERROR);
}

void Laser_Particle::draw(const vector< shared_ptr<Laser_Particle> > &particles,
                    shared_ptr<Program> prog)
{
    // Enable, bind, and send position array
    glEnableVertexAttribArray(prog->getAttribute("aPos"));
    glBindBuffer(GL_ARRAY_BUFFER, posBufID);
    glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_DYNAMIC_DRAW);
    glVertexAttribPointer(prog->getAttribute("aPos"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    // Enable, bind, and send alpha array
    glEnableVertexAttribArray(prog->getAttribute("aAlp"));
    glBindBuffer(GL_ARRAY_BUFFER, alpBufID);
    glBufferData(GL_ARRAY_BUFFER, alpBuf.size()*sizeof(float), &alpBuf[0], GL_DYNAMIC_DRAW);
    glVertexAttribPointer(prog->getAttribute("aAlp"), 1, GL_FLOAT, GL_FALSE, 0, 0);
    
    // Enable and bind color array
    glEnableVertexAttribArray(prog->getAttribute("aCol"));
    glBindBuffer(GL_ARRAY_BUFFER, colBufID);
    glVertexAttribPointer(prog->getAttribute("aCol"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    // Enable and bind scale array
    glEnableVertexAttribArray(prog->getAttribute("aSca"));
    glBindBuffer(GL_ARRAY_BUFFER, scaBufID);
    glVertexAttribPointer(prog->getAttribute("aSca"), 1, GL_FLOAT, GL_FALSE, 0, 0);
    
    // Draw
    glDrawArrays(GL_POINTS, 0, 3*particles.size());
    
    // Disable and unbind
    glDisableVertexAttribArray(prog->getAttribute("aSca"));
    glDisableVertexAttribArray(prog->getAttribute("aCol"));
    glDisableVertexAttribArray(prog->getAttribute("aAlp"));
    glDisableVertexAttribArray(prog->getAttribute("aPos"));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

