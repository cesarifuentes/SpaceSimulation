#include "Particle.h"

#include <iostream>

#include "GLSL.h"
#include "MatrixStack.h"
#include "Program.h"
#include "Texture.h"

using namespace std;
using namespace Eigen;

vector<float> Particle::posBuf;
vector<float> Particle::colBuf;
vector<float> Particle::alpBuf;
vector<float> Particle::scaBuf;
GLuint Particle::posBufID;
GLuint Particle::colBufID;
GLuint Particle::alpBufID;
GLuint Particle::scaBufID;

// Before this constructor is called, posBuf must be a valid vector<float>.
// I.e., Particle::init(n) must be called first.
Particle::Particle(int index) :
    color(&colBuf[3*index]),
    scale(scaBuf[index]),
    x(&posBuf[3*index]),
    alpha(alpBuf[index])
{
    
    this->index = index;

    color = Vector3f(1.0f,1.0f,1.0f);
    scale = 0.1; // randFloat(0.1f, 0.2f);
    
    // Send color data to GPU
    glBindBuffer(GL_ARRAY_BUFFER, colBufID);
    glBufferSubData(GL_ARRAY_BUFFER, 3*index*sizeof(float), 3*sizeof(float), color.data());
    
    // Send scale data to GPU
    glBindBuffer(GL_ARRAY_BUFFER, scaBufID);
    glBufferSubData(GL_ARRAY_BUFFER, index*sizeof(float), sizeof(float), &scale);
}

Particle::~Particle()
{
}

void Particle::rebirth(float t, const bool *keyToggles, glm::vec3 drone_position, glm::vec3 drone_angles)
{
    m = 1.0f;
    alpha = 0.2f;
    
    // m = randFloat(0.0f, m);
    // alpha = randFloat(0.0f, alpha);
    
    //========================================================================================
    //========================================================================================
    
    float r = randFloat(0, 0.05); // 0.1
    float c1, c2;
    float a = randFloat(0, 2*M_PI);
    c1 = r*cos(a);
    c2 = r*sin(a);
    x << c1, 0, c2;
    
    
    v = Vector3f(0, 0, 1.5)-x; // TODO: fix dis
//    v = Vector3f(0,0,0);
    
    
    /*
     
    // find random circle point on drone's x-y plane
    float r = randFloat(0, 0.05); // 0.1
    float c1, c2;
    float a = randFloat(0, 2*M_PI);
    c1 = r*cos(a);
    c2 = r*sin(a);
    glm::vec4 point = glm::vec4(c1, c2, 0, 0);
     
    // rotate vector by angles
    glm::quat rotation = glm::quat(drone_angles);
    point = glm::toMat4(rotation) * point;
     
    // add point to drone position
    Vector3f c = Vector3f(drone_position.x, drone_position.y, drone_position.z);
    c += Vector3f(point.x, point.y, point.z);
    
    // add the engine displacement from drone_position
    int size = (posBuf.size()/3.0/2.0);
    if (index < size*1) {
        // translate to left
        glm::vec4 r = glm::toMat4(rotation) * glm::vec4(-0.3,-0.25,1.0,0);
        c += Vector3f(r.x, r.y, r.z); // initial_engine_l_position
    } else if (index < size*2) {
        // translate to right
        glm::vec4 r = glm::toMat4(rotation) * glm::vec4(0.3,-0.25,1.0,0);
        c += Vector3f(r.x, r.y, r.z); // initial_engine_r_position
    }
    
    x = c;
    v = Vector3f(0, 0, 0);
    */
    
    //========================================================================================
    //========================================================================================

    lifespan = 1.0f;
    lifespan = randFloat(0.0f, lifespan);

    tEnd = t + lifespan;
}

void Particle::step(float t, float h, const Vector3f &g, const bool *keyToggles, glm::vec3 drone_position, glm::vec3 drone_angles, bool areEnginesRunning, bool didTrailDissapate)
{
    if(t > tEnd && areEnginesRunning) {
        rebirth(t, keyToggles, drone_position, drone_angles);
    }
    // Update alpha based on current time
    alpha = (tEnd-t)/lifespan;
    
    if (!areEnginesRunning && didTrailDissapate) {
        m = 0;
    } else {
        m = 1.0f;
    }
    
    //========================================================================================
    //========================================================================================
    // Accumulate forces
    Vector3f ft = Vector3f(0.0f, 0.0f, 0.0f);
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
    color.z() = 1.0f * ( (0.0 + (i-0.0) * randFloat(0.0, 1.0)) * (1-r) );
    
    // Send color data to GPU
    glBindBuffer(GL_ARRAY_BUFFER, colBufID);
    glBufferSubData(GL_ARRAY_BUFFER, 3*index*sizeof(float), 3*sizeof(float), color.data());

}

float Particle::randFloat(float l, float h)
{
    float r = rand() / (float)RAND_MAX;
    return (1.0f - r) * l + r * h;
}

void Particle::init(int n)
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

void Particle::draw(const vector< shared_ptr<Particle> > &particles,
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
