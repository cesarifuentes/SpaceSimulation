#ifndef Variables_h
#define Variables_h
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#ifndef _GLIBCXX_USE_NANOSLEEP
#define _GLIBCXX_USE_NANOSLEEP
#endif
#include <thread>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLSL.h"
#include "Program.h"
#include "Camera.h"
#include "MatrixStack.h"
#include "Particle.h"
#include "Smoke_Particle.h"
#include "Laser_Particle.h"
#include "Explosion_Particle.h"
#include "Trailing_Particle.h"
#include "Shape.h"
#include "Scene.h"
#include "Texture.h"
#include <map>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp> // printing matrix
#include "stb_image.h"

using namespace std;
using namespace Eigen;

bool keyToggles[256] = {false}; // only for English keyboards!

GLFWwindow *window; // Main application window
string RESOURCE_DIR = "../../resources"; // Where the resources are loaded from

std::atomic<bool> stop_flag;

//========================================================================================
//========================================================================================

shared_ptr<Camera> camera = make_shared<Camera>();
glm::vec3 lightPosWorld = glm::vec3(-10,0,0);

bool areEnginesRunning = false;
shared_ptr<Program> prog;

//========================================================================================
//========================================================================================

int PLAYER_HEALTH = 3;
int PLAYER_SCORE = 0;

//========================================================================================
//========================================================================================


// drone
shared_ptr<Shape> drone_shape;
shared_ptr<Texture> kdTex;
shared_ptr<Texture> ksTex;
shared_ptr<Texture> nmTex;

glm::vec3 initial_drone_position = glm::vec3(-2.7,-0.5,0.05);
float initial_drone_angle = -1*M_PI/2;
glm::vec3 drone_position = glm::vec3(0,0,30);
glm::vec3 current_drone_angles = glm::vec3(0,0,0);
glm::vec3 target_drone_angles;
float drone_speed = 0.3;
float drone_rotation_speed = M_PI/8.0; // 0.05*
glm::vec3 initial_engine_l_position = glm::vec3(-0.3,-0.25,1.0);
glm::vec3 initial_engine_r_position = glm::vec3(0.3,-0.25,1.0);
int stoppingTime = 3;

// earth
shared_ptr<Shape> sphere_shape;
shared_ptr<Texture> earth_kdTex;
shared_ptr<Texture> earth_ksTex;
shared_ptr<Texture> earth_nmTex;
glm::vec3 earth_position = glm::vec3(250,0,0);
float earth_scale = 50.0f;

// sun
shared_ptr<Texture> sun_kdTex;
shared_ptr<Texture> sun_ksTex;
shared_ptr<Texture> sun_nmTex;
glm::vec3 sun_position = lightPosWorld;
float sun_scale = 2.0f;

// asteroid
int NUM_INITIAL_ASTEROIDS = 100;
shared_ptr<Shape> asteroid_shape;
shared_ptr<Texture> asteroid_kdTex;
shared_ptr<Texture> asteroid_ksTex;
shared_ptr<Texture> asteroid_nmTex;
vector<pair<glm::vec3,glm::vec3>> asteroid_positions;

// satellite
int NUM_INITIAL_SATELLITES = 10;
shared_ptr<Shape> satellite_shape;
shared_ptr<Texture> satellite_kdTex;
shared_ptr<Texture> satellite_ksTex;
shared_ptr<Texture> satellite_nmTex;
//glm::vec3 satellite_position = glm::vec3(-2,4,-6);
//glm::vec3 satellite_angles = glm::vec3(0,-0.1,-0.8);
vector<pair<glm::vec3,glm::vec3>> satellite_positions;
float satellite_scale = 0.4;

// medkit
int NUM_INITIAL_MEDKITS = 5;
shared_ptr<Shape> medkit_shape;
shared_ptr<Texture> medkit_kdTex;
shared_ptr<Texture> medkit_ksTex;
shared_ptr<Texture> medkit_nmTex;
vector<pair<glm::vec3,glm::vec3>> medkit_positions;
float medkit_scale = 5e-3;
float initial_medkit_angle = M_PI/2;
float medkit_distance = 1.3;

// stars
int NUM_INITIAL_STARS = 5;
shared_ptr<Shape> star_shape;
shared_ptr<Texture> star_kdTex;
shared_ptr<Texture> star_ksTex;
shared_ptr<Texture> star_nmTex;
vector<pair<glm::vec3,glm::vec3>> star_positions;
float star_scale = 1e-2;
float star_scale_hud = (5e-3)/5;
float star_distance = 1.3;

//========================================================================================
//========================================================================================

shared_ptr<Program> prog_particle;
shared_ptr<Program> prog_uv_particle;
Eigen::Vector3f grav = Eigen::Vector3f(0.0f, -9.8f, 0.0f);
float t = 0.0f;
float h = 0.01f;
map<string, float> times;

// engine
vector< shared_ptr< Particle> > fire_particles;
shared_ptr<Texture> fireTex;
    float lastTimeEnginesStopped = -std::numeric_limits<float>::infinity();

// trail
vector< shared_ptr< Trailing_Particle> > trailing_particles;
shared_ptr<Texture> trailTex;

// smoke
vector< shared_ptr< Smoke_Particle> > smoke_particles;
shared_ptr<Texture> smokeTex;

// laser
vector< shared_ptr< Laser_Particle> > laser_particles;
shared_ptr<Texture> laserTex;
    bool isLaserActive = false;
    float lastTimeLaserShot = -std::numeric_limits<float>::infinity();
    glm::vec3 initial_laser_l_position = glm::vec3(-0.85,-0.38,-0.3);
    glm::vec3 initial_laser_r_position = glm::vec3(0.885,-0.38,-0.3);
    glm::vec3 laser_position = drone_position;
    glm::vec3 prev_drone_angles;
    float laser_speed = 0.6;
    float laser_reload_time = 3;

// explosion
vector< shared_ptr< Explosion_Particle> > explosion_particles;
shared_ptr<Texture> explosionTex;
    float lastTimeLaserHit = -std::numeric_limits<float>::infinity();
    glm::vec3 collisionPosition = glm::vec3(0,0,0);
    float explosion_duration = 3;

//========================================================================================
//========================================================================================

glm::mat4 getRotationMatrix(glm::vec3 angles)
{
    glm::quat rotation = glm::quat(angles);
    return glm::toMat4(rotation);
    // glm::mat4 RotationMatrix = glm::toMat4(drone_rotation);
}

static void error_callback(int error, const char *description)
{
    cerr << description << endl;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        stop_flag = true;
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

static void cursor_position_callback(GLFWwindow* window, double xmouse, double ymouse)
{
    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if(state == GLFW_PRESS) {
         camera->mouseMoved(xmouse, ymouse);
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    // Get the current mouse position.
    double xmouse, ymouse;
    glfwGetCursorPos(window, &xmouse, &ymouse);
    // Get current window size.
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    if(action == GLFW_PRESS) {
        bool shift = mods & GLFW_MOD_SHIFT;
        bool ctrl  = mods & GLFW_MOD_CONTROL;
        bool alt   = mods & GLFW_MOD_ALT;
        camera->mouseClicked(xmouse, ymouse, shift, ctrl, alt);
    }
}


//========================================================================================
//========================================================================================

// skybox
shared_ptr<Program> progSkybox;
std::shared_ptr<Shape> box;
unsigned int skyboxID;

unsigned int loadCubemap() {
    string dir = RESOURCE_DIR + "skybox/";
    vector<std::string> faces { "right.png", "left.png", "top.png", "bottom.png", "back.png", "front.png"};
    
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char *data = stbi_load((dir + faces[i]).c_str(), &width, &height, &nrChannels, 3); // changed from 0 to 3
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap tex failed to load at path: " << dir + faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return textureID;
}

#endif
