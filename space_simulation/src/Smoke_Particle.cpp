#include "Smoke_Particle.h"

#include <iostream>

#include "GLSL.h"
#include "MatrixStack.h"
#include "Program.h"
#include "Texture.h"

using namespace std;
using namespace Eigen;

vector<float> Smoke_Particle::posBuf;
vector<float> Smoke_Particle::colBuf;
vector<float> Smoke_Particle::alpBuf;
vector<float> Smoke_Particle::scaBuf;
GLuint Smoke_Particle::posBufID;
GLuint Smoke_Particle::colBufID;
GLuint Smoke_Particle::alpBufID;
GLuint Smoke_Particle::scaBufID;

// Before this constructor is called, posBuf must be a valid vector<float>.
// I.e., Particle::init(n) must be called first.
Smoke_Particle::Smoke_Particle(int index) :
    color(&colBuf[3*index]),
    scale(scaBuf[index]),
    x(&posBuf[3*index]),
    alpha(alpBuf[index])
{
    
    this->index = index;

    float black = 0.0;
    color = Vector3f(black,black,black);
    scale = randFloat(0.1f, 0.6f); // scale = 0.1; // randFloat(0.1f, 0.2f);
    
    float val = 0.3f;
    dir = Vector3f(randFloat(-1*val, val), randFloat(-1*val, val), randFloat(-1*val, val));
    
    // Send color data to GPU
    glBindBuffer(GL_ARRAY_BUFFER, colBufID);
    glBufferSubData(GL_ARRAY_BUFFER, 3*index*sizeof(float), 3*sizeof(float), color.data());
    
    // Send scale data to GPU
    glBindBuffer(GL_ARRAY_BUFFER, scaBufID);
    glBufferSubData(GL_ARRAY_BUFFER, index*sizeof(float), sizeof(float), &scale);
}

Smoke_Particle::~Smoke_Particle()
{
}

void Smoke_Particle::rebirth(float t, const bool *keyToggles)
{
    m = 1.0f;
    alpha = 0.2f;
    
    //========================================================================================
    //========================================================================================
    float r = 0.1;
    x << randFloat(-1*r, r), 0, randFloat(-1*r, r);
    v = Vector3f(0, 0, 0);
    //========================================================================================
    //========================================================================================

    lifespan = 5.0f;
    lifespan = randFloat(1.0f, lifespan);

    tEnd = t + lifespan;
}

void Smoke_Particle::step(float t, float h, const Vector3f &g, const bool *keyToggles)
{
    if(t > tEnd) {
        rebirth(t, keyToggles);
    }
    // Update alpha based on current time
    alpha = (tEnd-t)/lifespan;
    
    //========================================================================================
    //========================================================================================
    // Accumulate forces
    Vector3f f = dir;
    
    //========================================================================================
    //========================================================================================
    
    // Update velocity
    v += (h/m)*f;
    
    // Update position
    x += h*v;
    
    
//
//
//    // Update size
//    scale += 100; // TODO:
//
//    // Update color
//    float r = x.norm();
//    float inc = r/250.0;
//    color += Vector3f(inc,inc,inc); // TODO:
//
    
    
    // Send color data to GPU
    glBindBuffer(GL_ARRAY_BUFFER, colBufID);
    glBufferSubData(GL_ARRAY_BUFFER, 3*index*sizeof(float), 3*sizeof(float), color.data());

}

float Smoke_Particle::randFloat(float l, float h)
{
    float r = rand() / (float)RAND_MAX;
    return (1.0f - r) * l + r * h;
}

void Smoke_Particle::init(int n)
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

void Smoke_Particle::draw(const vector< shared_ptr<Smoke_Particle> > &particles,
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
