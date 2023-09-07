#include "Variables.h"

//#include <learnopengl/shader_m.h>

//========================================================================================
//========================================================================================

bool optimizeShaders = true;
bool should_Draw_Drone = true;

bool should_Draw_Asteroids = true;
bool should_Draw_Satellite = true;
bool should_Draw_Skybox = true;
bool should_Draw_Medkits = true;
bool should_Draw_Stars = true;

bool should_Draw_Grid = false;
bool should_Draw_Earth = false;
bool should_Draw_Sun = false;

//========================================================================================
//========================================================================================

void drawDrone(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> MV, int width, int height, glm::mat4 E) {
    if (should_Draw_Drone) {
        prog->bind();
        
            MV->pushMatrix();
        
                // TODO: Give slight implication angles
                // MV->multMatrix(getRotationMatrix(delta_angles));

                // Controlled Transormations
                MV->multMatrix(E);

                // Standard Transformations
                MV->rotate(initial_drone_angle, glm::vec3(0,1,0));
                MV->translate(initial_drone_position); // translate to origin
                
                kdTex->bind(prog->getUniform("kdTex"));
                if (!optimizeShaders) {
                    kdTex->bind(prog->getUniform("ksTex"));
                    kdTex->bind(prog->getUniform("nmTex"));
                }
                glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
                glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
                glUniform3f(prog->getUniform("ka"), 0.1f, 0.1f, 0.1f);
                glUniform3f(prog->getUniform("ks"), 0.1f, 0.1f, 0.1f);
                glUniform1f(prog->getUniform("s"), 200.0f);
                // glUniform3fv(prog->getUniform("lightPosCam"), 1, glm::value_ptr(lightPosCam));
            
                drone_shape->setProgram(prog);
                drone_shape->draw();
            
                kdTex->unbind();
                if (!optimizeShaders) {
                    ksTex->unbind();
                    nmTex->unbind();
                }
        
            MV->popMatrix();
        
        prog->unbind();
    }
}

void drawFlare(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> MV, int width, int height, glm::mat4 E) {
    if (should_Draw_Drone) {
        
        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        
        prog_particle->bind();
        
            MV->pushMatrix();
                // Controlled Transormations
                MV->multMatrix(E);
                // Standard Transformations
                MV->translate(initial_engine_l_position); // translate to origin
                fireTex->bind(prog_particle->getUniform("texture0"));
                glUniformMatrix4fv(prog_particle->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
                glUniformMatrix4fv(prog_particle->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
                glUniform2f(prog_particle->getUniform("screenSize"), (float)width, (float)height);
                Particle::draw(fire_particles, prog_particle);
                fireTex->unbind();
            MV->popMatrix();
        
            MV->pushMatrix();
                // Controlled Transormations
                MV->multMatrix(E);
                // Standard Transformations
                MV->translate(initial_engine_r_position); // translate to origin
                fireTex->bind(prog_particle->getUniform("texture0"));
                glUniformMatrix4fv(prog_particle->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
                glUniformMatrix4fv(prog_particle->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
                glUniform2f(prog_particle->getUniform("screenSize"), (float)width, (float)height);
                Particle::draw(fire_particles, prog_particle);
                fireTex->unbind();
            MV->popMatrix();
        
        prog_particle->unbind();
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        
    }
}

void drawTrailing(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> MV, int width, int height) {
    if (should_Draw_Drone) {
        
        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        
        prog_particle->bind();
        
            MV->pushMatrix();
                trailTex->bind(prog_particle->getUniform("texture0"));
                glUniformMatrix4fv(prog_particle->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
                glUniformMatrix4fv(prog_particle->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
                glUniform2f(prog_particle->getUniform("screenSize"), (float)width, (float)height);
                Trailing_Particle::draw(trailing_particles, prog_particle);
                trailTex->unbind();
            MV->popMatrix();
        
        prog_particle->unbind();
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        
    }
}

void drawEarth(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> MV, int width, int height) {
    if (should_Draw_Earth) {
        prog->bind();
        
            MV->pushMatrix();

                // Controlled Transformations
                MV->translate(earth_position);
                MV->scale(earth_scale);
        
                earth_kdTex->bind(prog->getUniform("kdTex"));
                if (!optimizeShaders) {
                    earth_ksTex->bind(prog->getUniform("ksTex"));
                    earth_nmTex->bind(prog->getUniform("nmTex"));
                }
                glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
                glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
                glUniform3f(prog->getUniform("ka"), 0.1f, 0.1f, 0.1f);
                glUniform3f(prog->getUniform("ks"), 0.1f, 0.1f, 0.1f);
                glUniform1f(prog->getUniform("s"), 200.0f);
                // glUniform3fv(prog->getUniform("lightPosCam"), 1, glm::value_ptr(lightPosCam));
            
                sphere_shape->setProgram(prog);
                sphere_shape->draw();
            
                earth_kdTex->unbind();
                if (!optimizeShaders) {
                    earth_ksTex->unbind();
                    earth_nmTex->unbind();
                }
        
            MV->popMatrix();
        
        prog->unbind();
    }
}

void drawSun(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> MV, int width, int height) {
    if (should_Draw_Sun) {
        prog->bind();
        
            MV->pushMatrix();

                // Controlled Transformations
                MV->translate(sun_position);
                MV->scale(sun_scale);
        
                sun_kdTex->bind(prog->getUniform("kdTex"));
                if (!optimizeShaders) {
                    sun_ksTex->bind(prog->getUniform("ksTex"));
                    sun_nmTex->bind(prog->getUniform("nmTex"));
                }
                glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
                glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
                glUniform3f(prog->getUniform("ka"), 0.1f, 0.1f, 0.1f);
                glUniform3f(prog->getUniform("ks"), 0.1f, 0.1f, 0.1f);
                glUniform1f(prog->getUniform("s"), 0.0f);
                // glUniform3fv(prog->getUniform("lightPosCam"), 1, glm::value_ptr(lightPosCam));
            
                sphere_shape->setProgram(prog);
                sphere_shape->draw();
            
                sun_kdTex->unbind();
                if (!optimizeShaders) {
                    sun_ksTex->unbind();
                    sun_nmTex->unbind();
                }
        
            MV->popMatrix();
        
        prog->unbind();
    }
}

void drawAsteroids(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> MV, int width, int height) {
    if (should_Draw_Asteroids) {
        prog->bind();
            
            for (int i=0; i<asteroid_positions.size(); i++) {
                
                MV->pushMatrix();

                    // Controlled Transformations
                    MV->translate(asteroid_positions[i].first);
                    MV->multMatrix(getRotationMatrix(asteroid_positions[i].second));
                    // MV->rotate
                    // MV->scale(sun_scale);
            
                    asteroid_kdTex->bind(prog->getUniform("kdTex"));
                    if (!optimizeShaders) {
                        asteroid_ksTex->bind(prog->getUniform("ksTex"));
                        asteroid_nmTex->bind(prog->getUniform("nmTex"));
                    }
                    glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
                    glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
                    glUniform3f(prog->getUniform("ka"), 0.1f, 0.1f, 0.1f);
                    glUniform3f(prog->getUniform("ks"), 0.1f, 0.1f, 0.1f);
                    glUniform1f(prog->getUniform("s"), 200.0f);
                    // glUniform3fv(prog->getUniform("lightPosCam"), 1, glm::value_ptr(lightPosCam));
            
                    asteroid_shape->setProgram(prog);
                    asteroid_shape->draw();
                
                    asteroid_kdTex->unbind();
                    if (!optimizeShaders) {
                        asteroid_ksTex->unbind();
                        asteroid_nmTex->unbind();
                    }
            
                MV->popMatrix();

            }
        
        prog->unbind();
    }
    
}

void drawSatellite(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> MV, int width, int height) {
    if (should_Draw_Satellite) {
        prog->bind();
        
            for (int i=0; i<satellite_positions.size(); i++) {
            
                MV->pushMatrix();

                    MV->translate(satellite_positions[i].first);
                    MV->multMatrix(getRotationMatrix(satellite_positions[i].second));
                    MV->scale(satellite_scale);
                            
                    satellite_kdTex->bind(prog->getUniform("kdTex"));
                    if (!optimizeShaders) {
                        satellite_ksTex->bind(prog->getUniform("ksTex"));
                        satellite_nmTex->bind(prog->getUniform("nmTex"));
                    }
                    glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
                    glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
                    glUniform3f(prog->getUniform("ka"), 0.1f, 0.1f, 0.1f);
                    glUniform3f(prog->getUniform("ks"), 0.1f, 0.1f, 0.1f);
                    glUniform1f(prog->getUniform("s"), 200.0f);
                    // glUniform3fv(prog->getUniform("lightPosCam"), 1, glm::value_ptr(lightPosCam));
                
                    satellite_shape->setProgram(prog);
                    satellite_shape->draw();
                
                    satellite_kdTex->unbind();
                    if (!optimizeShaders) {
                        satellite_ksTex->unbind();
                        satellite_nmTex->unbind();
                    }
            
                MV->popMatrix();
            
            }
        
        prog->unbind();
    }
    
}

void drawSmoke(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> MV, int width, int height) {
    if (should_Draw_Satellite) {
        
        for (int i=0; i<satellite_positions.size(); i++) {
            
            glEnable(GL_BLEND);
            glDepthMask(GL_FALSE);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            prog_particle->bind();
            
                MV->pushMatrix();
            
                    MV->translate(satellite_positions[i].first);
            
                    smokeTex->bind(prog_particle->getUniform("texture0"));
                    glUniformMatrix4fv(prog_particle->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
                    glUniformMatrix4fv(prog_particle->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
                    glUniform2f(prog_particle->getUniform("screenSize"), (float)width, (float)height);
                    Smoke_Particle::draw(smoke_particles, prog_particle);
                    smokeTex->unbind();
                MV->popMatrix();
            
            prog_particle->unbind();
            glDepthMask(GL_TRUE);
            glDisable(GL_BLEND);
        
        }
        
    }
}

void drawLasers(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> MV, int width, int height) {
    if (should_Draw_Drone) {
            
        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        
        prog_particle->bind();
        
        MV->pushMatrix();
        
            // Controlled Transormations
            MV->translate(laser_position);
        
            if (isLaserActive) {
                // rotate wrt previous drone rotation
                MV->multMatrix(getRotationMatrix(prev_drone_angles));
            } else {
                // rotate wrt current drone rotation
                MV->multMatrix(getRotationMatrix(current_drone_angles));
            }
            
            MV->pushMatrix();
                // Standard Transformations
                MV->translate(initial_laser_l_position); // translate to origin
                laserTex->bind(prog_particle->getUniform("texture0"));
                glUniformMatrix4fv(prog_particle->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
                glUniformMatrix4fv(prog_particle->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
                glUniform2f(prog_particle->getUniform("screenSize"), (float)width, (float)height);
                Laser_Particle::draw(laser_particles, prog_particle);
                laserTex->unbind();
            MV->popMatrix();
        
            MV->pushMatrix();
                // Standard Transformations
                MV->translate(initial_laser_r_position); // translate to origin
                laserTex->bind(prog_particle->getUniform("texture0"));
                glUniformMatrix4fv(prog_particle->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
                glUniformMatrix4fv(prog_particle->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
                glUniform2f(prog_particle->getUniform("screenSize"), (float)width, (float)height);
                Laser_Particle::draw(laser_particles, prog_particle);
                laserTex->unbind();
            MV->popMatrix();
            
            prog_particle->unbind();
            glDepthMask(GL_TRUE);
            glDisable(GL_BLEND);
        
        MV->popMatrix();
        
    }
}

void drawExplosion(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> MV, int width, int height) {
    
//    bool isExplosionActive = t < (lastTimeLaserHit + explosion_duration);
    
    if (should_Draw_Asteroids) {
        
        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        
        prog_particle->bind();
        
            MV->pushMatrix();
        
        
//        madeCollision = true;
//        collisionPosition = asteroid_positions[i].first;
        
//        laser_reload_time
        
                MV->translate(collisionPosition);
        
                explosionTex->bind(prog_particle->getUniform("texture0"));
                glUniformMatrix4fv(prog_particle->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
                glUniformMatrix4fv(prog_particle->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
                glUniform2f(prog_particle->getUniform("screenSize"), (float)width, (float)height);
                Explosion_Particle::draw(explosion_particles, prog_particle);
                explosionTex->unbind();
        
                // TODO: draw 2 more
        
        
            MV->popMatrix();
        
        prog_particle->unbind();
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
            
    }
}

void drawGrid(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> MV) {
    if (should_Draw_Grid) {
        // Draw the frame and the grid with OpenGL 1.x (no GLSL)
        
        // Setup the projection matrix
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadMatrixf(glm::value_ptr(P->topMatrix()));
        
        // Setup the modelview matrix
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadMatrixf(glm::value_ptr(MV->topMatrix()));
        
        // Draw frame
        glLineWidth(2);
        glBegin(GL_LINES);
        glColor3f(1, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(1, 0, 0);
        glColor3f(0, 1, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 1, 0);
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 1);
        glEnd();
        glLineWidth(1);
        
        // Draw grid
        float gridSizeHalf = 20.0f;
        int gridNx = 40;
        int gridNz = 40;
        glLineWidth(1);
        glColor3f(0.8f, 0.8f, 0.8f);
        glBegin(GL_LINES);
        for(int i = 0; i < gridNx+1; ++i) {
            float alpha = i / (float)gridNx;
            float x = (1.0f - alpha) * (-gridSizeHalf) + alpha * gridSizeHalf;
            glVertex3f(x, 0, -gridSizeHalf);
            glVertex3f(x, 0,  gridSizeHalf);
        }
        for(int i = 0; i < gridNz+1; ++i) {
            float alpha = i / (float)gridNz;
            float z = (1.0f - alpha) * (-gridSizeHalf) + alpha * gridSizeHalf;
            glVertex3f(-gridSizeHalf, 0, z);
            glVertex3f( gridSizeHalf, 0, z);
        }
        glEnd();
        
        // Pop modelview matrix
        glPopMatrix();
        
        // Pop projection matrix
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        
    }
}

void drawMedkits(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> MV, int width, int height, bool isHUD) {
    
    float t = glfwGetTime();
    
    if (should_Draw_Medkits) {
        prog->bind();
            
            int max;
            if (!isHUD) {
                max = medkit_positions.size();
            } else {
                max = PLAYER_HEALTH;
            }
        
            for (int i=0; i<max ; i++) {
                
                MV->pushMatrix();
                    
                    if (!isHUD) {
                        // Controlled Transformations
                        MV->translate(medkit_positions[i].first);
                        MV->multMatrix(getRotationMatrix(medkit_positions[i].second));
                        MV->rotate(t, glm::vec3(0,1,0)); // with time
                        // Standard Transformations
                        MV->rotate(initial_medkit_angle, glm::vec3(0,0,1));
                        MV->scale(medkit_scale);
                    } else {
                        float x = 0.1; // x = 0.1
                        MV->translate(x * i * glm::vec3(1,0,0)); // distance between medkits
                        MV->translate(glm::vec3(-0.94, 0.93, 0)); // push backwards // -1, 0.4, 0
                        MV->rotate(initial_medkit_angle, glm::vec3(0,1,0));
                        MV->rotate(initial_medkit_angle, glm::vec3(0,0,1));
                        MV->scale(medkit_scale/15); // 8
                    }
                    
                    medkit_kdTex->bind(prog->getUniform("kdTex"));
                    if (!optimizeShaders) {
                        medkit_ksTex->bind(prog->getUniform("ksTex"));
                        medkit_nmTex->bind(prog->getUniform("nmTex"));
                    }
                
                    glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
                    glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
                    glUniform3f(prog->getUniform("ka"), 0.1f, 0.1f, 0.1f);
                    glUniform3f(prog->getUniform("ks"), 0.1f, 0.1f, 0.1f);
                    glUniform1f(prog->getUniform("s"), 200.0f);
                    // glUniform3fv(prog->getUniform("lightPosCam"), 1, glm::value_ptr(lightPosCam));
            
                    medkit_shape->setProgram(prog);
                    medkit_shape->draw();
                
                    medkit_kdTex->unbind();
                    if (!optimizeShaders) {
                        medkit_ksTex->unbind();
                        medkit_nmTex->unbind();
                    }
                            
                MV->popMatrix();

            }
        
        prog->unbind();
    }
    
}

void drawStars(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> MV, int width, int height, bool isHUD) {
    
    float t = glfwGetTime();
    
    if (should_Draw_Stars) {
        prog->bind();
            
            int max;
            if (!isHUD) {
                max = star_positions.size();
            } else {
                max = PLAYER_SCORE;
            }
        
            for (int i=0; i<max ; i++) {
                
                MV->pushMatrix();
                    
                    if (!isHUD) {
                        // Controlled Transformations
                        MV->translate(star_positions[i].first);
                        MV->multMatrix(getRotationMatrix(star_positions[i].second));
                        MV->rotate(t, glm::vec3(0,1,0)); // with time
                        // Standard Transformations
                        // MV->rotate(initial_medkit_angle, glm::vec3(0,0,1));
                        MV->scale(star_scale);
                    } else {
                        float x = 0.1; // 0.15
                        MV->translate(x * i * glm::vec3(1,0,0)); // distance between medkits
                        MV->translate(glm::vec3(-0.94, 0.83, 0)); // push backwards // -1, 0.4, 0
                        
                        // -0.94, 0.93
                        // MV->rotate(initial_medkit_angle, glm::vec3(0,1,0));
                        // MV->rotate(initial_medkit_angle, glm::vec3(0,0,1));
                        MV->scale(star_scale_hud); // 2.5
                    }
                    
                    star_kdTex->bind(prog->getUniform("kdTex"));
                    if (!optimizeShaders) {
                        star_ksTex->bind(prog->getUniform("ksTex"));
                        star_nmTex->bind(prog->getUniform("nmTex"));
                    }
                
                    glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
                    glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
                    glUniform3f(prog->getUniform("ka"), 0.1f, 0.1f, 0.1f);
                    glUniform3f(prog->getUniform("ks"), 0.1f, 0.1f, 0.1f);
                    glUniform1f(prog->getUniform("s"), 0.0f); // 200.0f
                    // glUniform3fv(prog->getUniform("lightPosCam"), 1, glm::value_ptr(lightPosCam));
            
                    star_shape->setProgram(prog);
                    star_shape->draw();
                
                    star_kdTex->unbind();
                    if (!optimizeShaders) {
                        star_ksTex->unbind();
                        star_nmTex->unbind();
                    }
                            
                MV->popMatrix();

            }
        
        prog->unbind();
    }
    
}

void drawSkybox(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> MV) {
    if (should_Draw_Skybox) {
        glDisable(GL_DEPTH_TEST);
        progSkybox->bind();
        MV->pushMatrix();
            
            MV->translate(drone_position);
            MV->scale(100.0f);
            
            glUniformMatrix4fv(progSkybox->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
            glUniformMatrix4fv(progSkybox->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxID);
            glUniform1i(progSkybox->getUniform("skybox"), 0);
        
            box->setProgram(progSkybox);
            box->draw();
            
        MV->popMatrix();
        progSkybox->unbind();
        glEnable(GL_DEPTH_TEST);
    }
}

void drawHUD(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> MV, int width, int height) {
    
    if (!keyToggles[(unsigned)'p']) {
        // Send the Light Position for HUD
        prog->bind();
        glUniform3fv(prog->getUniform("lightPos"), 1, glm::value_ptr((glm::vec3(0,0,-10))));
        prog->unbind();
        
        // Draw HUD
        glMatrixMode(GL_PROJECTION);
        drawMedkits(P, MV, width, height, true);
        drawStars(P, MV, width, height, true);
        glMatrixMode(GL_MODELVIEW);
    }

}

//========================================================================================
//========================================================================================

void setProgram(shared_ptr<Program> prog) {
    prog->setShaderNames(RESOURCE_DIR + "drone_vert.glsl", RESOURCE_DIR + "drone_frag.glsl");
    prog->setVerbose(true); // Set this to true when debugging.
    prog->init();
    prog->addAttribute("aPos");
    prog->addAttribute("aNor");
    prog->addAttribute("aTex");
    prog->addUniform("P");
    prog->addUniform("MV");
    prog->addUniform("ka");
    prog->addUniform("ks");
    prog->addUniform("s");
    prog->addUniform("kdTex");
    prog->addUniform("ksTex");
    prog->addUniform("nmTex");
    prog->addUniform("lightPos");
}

void setParticleProgram(shared_ptr<Program> prog_particle) {
    prog_particle->setShaderNames(RESOURCE_DIR + "particle_vert.glsl", RESOURCE_DIR + "particle_frag.glsl");
    prog_particle->setVerbose(true);
    prog_particle->init();
    prog_particle->addAttribute("aPos");
    prog_particle->addAttribute("aAlp");
    prog_particle->addAttribute("aCol");
    prog_particle->addAttribute("aSca");
    prog_particle->addUniform("P");
    prog_particle->addUniform("MV");
    prog_particle->addUniform("screenSize");
    prog_particle->addUniform("texture0");
}

void initDrone() {
    if (should_Draw_Drone) {
        
        kdTex = make_shared<Texture>();
        kdTex->setFilename(RESOURCE_DIR + "COLOR_8K_DRON_.png");
        kdTex->init();
        kdTex->setUnit(0);
        kdTex->setWrapModes(GL_REPEAT, GL_REPEAT);
        
        if (!optimizeShaders) {
            ksTex = make_shared<Texture>();
            ksTex->setFilename(RESOURCE_DIR + "LIGHT_8K_DRON_.png");
            ksTex->init();
            ksTex->setUnit(1);
            ksTex->setWrapModes(GL_REPEAT, GL_REPEAT);
            
            nmTex = make_shared<Texture>();
            nmTex->setFilename(RESOURCE_DIR + "NM_8K_DRON_.png");
            nmTex->init();
            nmTex->setUnit(2);
            nmTex->setWrapModes(GL_REPEAT, GL_REPEAT);
        }
        
        drone_shape = make_shared<Shape>();
        drone_shape->loadMesh(RESOURCE_DIR + "Drone.obj");
        drone_shape->init();
        
    }
}

void initEarth() {
    if (should_Draw_Earth) {
        
        earth_kdTex = make_shared<Texture>();
        earth_kdTex->setFilename(RESOURCE_DIR + "8k_earth_daymap.jpg");
        earth_kdTex->init();
        earth_kdTex->setUnit(0);
        earth_kdTex->setWrapModes(GL_REPEAT, GL_REPEAT);
        
        if (!optimizeShaders) {
            earth_ksTex = make_shared<Texture>();
            earth_ksTex->setFilename(RESOURCE_DIR + "8k_earth_specular_map.jpg");
            earth_ksTex->init();
            earth_ksTex->setUnit(1);
            earth_ksTex->setWrapModes(GL_REPEAT, GL_REPEAT);
            
            earth_nmTex = make_shared<Texture>();
            earth_nmTex->setFilename(RESOURCE_DIR + "8k_earth_normal_map.jpg");
            earth_nmTex->init();
            earth_nmTex->setUnit(2);
            earth_nmTex->setWrapModes(GL_REPEAT, GL_REPEAT);
        }
        
        sphere_shape = make_shared<Shape>();
        sphere_shape->loadMesh(RESOURCE_DIR + "Sphere.obj");
        sphere_shape->init();
        
    }
}

void initSun() {
    if (should_Draw_Sun) {

        sun_kdTex = make_shared<Texture>();
        sun_kdTex->setFilename(RESOURCE_DIR + "8k_sun.jpg");
        sun_kdTex->init();
        sun_kdTex->setUnit(0);
        sun_kdTex->setWrapModes(GL_REPEAT, GL_REPEAT);
        
        if (!optimizeShaders) {
            sun_ksTex = make_shared<Texture>();
            sun_ksTex->setFilename(RESOURCE_DIR + "8k_sun.jpg");
            sun_ksTex->init();
            sun_ksTex->setUnit(1);
            sun_ksTex->setWrapModes(GL_REPEAT, GL_REPEAT);
            
            sun_nmTex = make_shared<Texture>();
            sun_nmTex->setFilename(RESOURCE_DIR + "8k_sun.jpg");
            sun_nmTex->init();
            sun_nmTex->setUnit(2);
            sun_nmTex->setWrapModes(GL_REPEAT, GL_REPEAT);
        }
        
        sphere_shape = make_shared<Shape>();
        sphere_shape->loadMesh(RESOURCE_DIR + "Sphere.obj");
        sphere_shape->init();
        
    }
}

void initAsteroids() {
    if (should_Draw_Asteroids) {

        asteroid_kdTex = make_shared<Texture>();
        asteroid_kdTex->setFilename(RESOURCE_DIR + "ASTEROID_LOW_POLY_4_COLOR_.png");
        asteroid_kdTex->init();
        asteroid_kdTex->setUnit(0);
        asteroid_kdTex->setWrapModes(GL_REPEAT, GL_REPEAT);
        
        if (!optimizeShaders) {
            asteroid_ksTex = make_shared<Texture>();
            asteroid_ksTex->setFilename(RESOURCE_DIR + "ASTEROID_LOW_POLY_4_COLOR_.png");
            asteroid_ksTex->init();
            asteroid_ksTex->setUnit(1);
            asteroid_ksTex->setWrapModes(GL_REPEAT, GL_REPEAT);
            
            asteroid_nmTex = make_shared<Texture>();
            asteroid_nmTex->setFilename(RESOURCE_DIR + "ASTEROID_LOW_POLY_4_NM_.png");
            asteroid_nmTex->init();
            asteroid_nmTex->setUnit(2);
            asteroid_nmTex->setWrapModes(GL_REPEAT, GL_REPEAT);
        }
        
        asteroid_shape = make_shared<Shape>();
        asteroid_shape->loadMesh(RESOURCE_DIR + "Asteroid.obj");
        asteroid_shape->init();
        
    }
}

void initSatellite() {
    if (should_Draw_Satellite) {
        
        satellite_kdTex = make_shared<Texture>();
        satellite_kdTex->setFilename(RESOURCE_DIR + "satellite_Satélite_BaseColor.jpg");
        satellite_kdTex->init();
        satellite_kdTex->setUnit(0);
        satellite_kdTex->setWrapModes(GL_REPEAT, GL_REPEAT);
        
        if (!optimizeShaders) {
            satellite_ksTex = make_shared<Texture>();
            satellite_ksTex->setFilename(RESOURCE_DIR + "satellite_Satélite_BaseColor.jpg");
            satellite_ksTex->init();
            satellite_ksTex->setUnit(1);
            satellite_ksTex->setWrapModes(GL_REPEAT, GL_REPEAT);
            
            satellite_nmTex = make_shared<Texture>();
            satellite_nmTex->setFilename(RESOURCE_DIR + "satellite_Satélite_Normal.jpg");
            satellite_nmTex->init();
            satellite_nmTex->setUnit(2);
            satellite_nmTex->setWrapModes(GL_REPEAT, GL_REPEAT);
        }
        
        satellite_shape = make_shared<Shape>();
        satellite_shape->loadMesh(RESOURCE_DIR + "Satellite.obj");
        satellite_shape->init();
        
    }
    
}

void initFlareParticles() {
    if (should_Draw_Drone) {
        
        fireTex = make_shared<Texture>();
        fireTex->setFilename(RESOURCE_DIR + "alpha.jpg");
        fireTex->init();
        fireTex->setUnit(0);
        fireTex->setWrapModes(GL_REPEAT, GL_REPEAT);
        
        int n = 300;
        Particle::init(n);
        for(int i = 0; i < n; ++i) {
            auto p = make_shared<Particle>(i);
            fire_particles.push_back(p);
            p->rebirth(0.0f, keyToggles, drone_position, current_drone_angles);
        }
        
    }
}

void initTrailParticles() {
    if (should_Draw_Drone) {

        trailTex = make_shared<Texture>();
        trailTex->setFilename(RESOURCE_DIR + "alpha.jpg");
        trailTex->init();
        trailTex->setUnit(0);
        trailTex->setWrapModes(GL_REPEAT, GL_REPEAT);
        
        int n = 1000;
        Trailing_Particle::init(n);
        for(int i = 0; i < n; ++i) {
            auto p = make_shared<Trailing_Particle>(i);
            trailing_particles.push_back(p);
            p->rebirth(0.0f, keyToggles, drone_position, current_drone_angles, areEnginesRunning, true);
        }
    }

}

void initSmokeParticles() {
    if (should_Draw_Satellite) {
        
        smokeTex = make_shared<Texture>();
        smokeTex->setFilename(RESOURCE_DIR + "smoke_07.png");
        smokeTex->init();
        smokeTex->setUnit(0);
        smokeTex->setWrapModes(GL_REPEAT, GL_REPEAT);
        
        int n = 500;
        Smoke_Particle::init(n);
        for(int i = 0; i < n; ++i) {
            auto p = make_shared<Smoke_Particle>(i);
            smoke_particles.push_back(p);
            p->rebirth(0.0f, keyToggles);
        }
        
    }

}

void initLaserParticles() {
    if (should_Draw_Drone) {

        laserTex = make_shared<Texture>();
        laserTex->setFilename(RESOURCE_DIR + "alpha.jpg");
        laserTex->init();
        laserTex->setUnit(0);
        laserTex->setWrapModes(GL_REPEAT, GL_REPEAT);
        
        int n = 1000;
        Laser_Particle::init(n);
        for(int i = 0; i < n; ++i) {
            auto p = make_shared<Laser_Particle>(i);
            laser_particles.push_back(p);
            p->rebirth(0.0f, keyToggles);
        }
        
    }
}

void initExplosionParticles() {
    // Explosion
    if (should_Draw_Asteroids) {

        explosionTex = make_shared<Texture>();
        explosionTex->setFilename(RESOURCE_DIR + "smoke_08.png"); // smoke_08.png
        explosionTex->init();
        explosionTex->setUnit(0);
        explosionTex->setWrapModes(GL_REPEAT, GL_REPEAT);
        
        int n = 400;
        Explosion_Particle::init(n);
        for(int i = 0; i < n; ++i) {
            auto p = make_shared<Explosion_Particle>(i);
            explosion_particles.push_back(p);
            p->rebirth(0.0f, keyToggles, true);
        }
        
    }
}

void initSkybox() {
    if (should_Draw_Skybox) {
        
        progSkybox = make_shared<Program>();
        progSkybox->setShaderNames(RESOURCE_DIR + "space_vert.glsl", RESOURCE_DIR + "space_frag.glsl");
        progSkybox->setVerbose(true);
        progSkybox->init();
        progSkybox->addUniform("P");
        progSkybox->addUniform("MV");
        progSkybox->addUniform("skybox");
        progSkybox->addAttribute("aPos");
        progSkybox->addAttribute("aNor");
        progSkybox->addAttribute("aTex"); // needed
        progSkybox->setVerbose(false);
        
        box = make_shared<Shape>();
        box->loadMesh(RESOURCE_DIR + "cube.obj");
        box->init();
        
        skyboxID = loadCubemap();
//        cout << "skyboxID: " << skyboxID << endl;
        
    }
}

void getRandomAsteroidPositions() {
    // Random Asteroid Positions
    int min, max;
    for (int i=0; i<NUM_INITIAL_ASTEROIDS; i++) {
        min = -20;
        max = 20;
        float randPos1 = 1.0f * (rand()%(max-min + 1) + min);
        float randPos2 = 1.0f * (rand()%(max-min + 1) + min);
        float randPos3 = 1.0f * (rand()%(max-min + 1) + min);
        glm::vec3 asteroid_position = glm::vec3(randPos1,randPos2,randPos3);
        min = 0;
        max = 2*M_PI;
        float randAng1 = 1.0f * (rand()%(max-min + 1) + min);
        glm::vec3 asteroid_angle = glm::vec3(randAng1,randAng1,randAng1);
        asteroid_positions.push_back(make_pair(asteroid_position,asteroid_angle));
    }
}

void getRandomSatellitePositions() {
    // Random Satellite Positions
    int min, max;
    for (int i=0; i<NUM_INITIAL_SATELLITES; i++) {
        min = -20;
        max = 20;
        float randPos1 = 1.0f * (rand()%(max-min + 1) + min);
        float randPos2 = 1.0f * (rand()%(max-min + 1) + min);
        float randPos3 = 1.0f * (rand()%(max-min + 1) + min);
        glm::vec3 satellite_position = glm::vec3(randPos1,randPos2,randPos3);
        min = 0;
        max = 2*M_PI;
        float randAng1 = 1.0f * (rand()%(max-min + 1) + min);
        glm::vec3 satellite_angle = glm::vec3(randAng1,randAng1,randAng1);
        satellite_positions.push_back(make_pair(satellite_position,satellite_angle));
    }
}

void initMedkits() {
    if (should_Draw_Medkits) {

        medkit_kdTex = make_shared<Texture>();
        medkit_kdTex->setFilename(RESOURCE_DIR + "Health Pack_Albedo.png");
        medkit_kdTex->init();
        medkit_kdTex->setUnit(0);
        medkit_kdTex->setWrapModes(GL_REPEAT, GL_REPEAT);
        
        if (!optimizeShaders) {
            medkit_ksTex = make_shared<Texture>();
            medkit_ksTex->setFilename(RESOURCE_DIR + "Health Pack_Metallic.png");
            medkit_ksTex->init();
            medkit_ksTex->setUnit(1);
            medkit_ksTex->setWrapModes(GL_REPEAT, GL_REPEAT);
            
            medkit_nmTex = make_shared<Texture>();
            medkit_nmTex->setFilename(RESOURCE_DIR + "Health Pack_Normal.png");
            medkit_nmTex->init();
            medkit_nmTex->setUnit(2);
            medkit_nmTex->setWrapModes(GL_REPEAT, GL_REPEAT);
        }
        
        medkit_shape = make_shared<Shape>();
        medkit_shape->loadMesh(RESOURCE_DIR + "Medkit.obj");
        medkit_shape->init();
        
    }
}

void getRandomMedkitPositions() {
    int min, max;
    for (int i=0; i<NUM_INITIAL_MEDKITS; i++) {
        min = -20;
        max = 20;
        float randPos1 = 1.0f * (rand()%(max-min + 1) + min);
        float randPos2 = 1.0f * (rand()%(max-min + 1) + min);
        float randPos3 = 1.0f * (rand()%(max-min + 1) + min);
        glm::vec3 medkit_position = glm::vec3(randPos1,randPos2,randPos3);
        min = 0;
        max = 2*M_PI;
        float randAng1 = 1.0f * (rand()%(max-min + 1) + min);
        glm::vec3 medkit_angle = glm::vec3(0,randAng1,0);
        medkit_positions.push_back(make_pair(medkit_position,medkit_angle));
    }
}

void initStars() {
    if (should_Draw_Stars) {

        star_kdTex = make_shared<Texture>();
        star_kdTex->setFilename(RESOURCE_DIR + "txtr01.jpeg");
        star_kdTex->init();
        star_kdTex->setUnit(0);
        star_kdTex->setWrapModes(GL_REPEAT, GL_REPEAT);
        
        if (!optimizeShaders) {
            star_ksTex = make_shared<Texture>();
            star_ksTex->setFilename(RESOURCE_DIR + "txtr01.jpeg");
            star_ksTex->init();
            star_ksTex->setUnit(1);
            star_ksTex->setWrapModes(GL_REPEAT, GL_REPEAT);
            
            star_nmTex = make_shared<Texture>();
            star_nmTex->setFilename(RESOURCE_DIR + "txtr01.jpeg");
            star_nmTex->init();
            star_nmTex->setUnit(2);
            star_nmTex->setWrapModes(GL_REPEAT, GL_REPEAT);
        }
        
        star_shape = make_shared<Shape>();
        star_shape->loadMesh(RESOURCE_DIR + "Star.obj");
        star_shape->init();
        
    }
}

void getRandomStarPositions() {
    int min, max;
    for (int i=0; i<NUM_INITIAL_STARS; i++) {
        min = -20;
        max = 20;
        float randPos1 = 1.0f * (rand()%(max-min + 1) + min);
        float randPos2 = 1.0f * (rand()%(max-min + 1) + min);
        float randPos3 = 1.0f * (rand()%(max-min + 1) + min);
        glm::vec3 star_position = glm::vec3(randPos1,randPos2,randPos3);
        min = 0;
        max = 2*M_PI;
        float randAng1 = 1.0f * (rand()%(max-min + 1) + min);
        glm::vec3 star_angle = glm::vec3(0,randAng1,0);
        star_positions.push_back(make_pair(star_position,star_angle));
    }
}

//========================================================================================
//========================================================================================

void updateLaserPosition() {
    bool canShootLaser = t-lastTimeLaserShot>laser_reload_time;
    isLaserActive = !canShootLaser;
    if (isLaserActive) {
        glm::vec3 d = glm::vec3(getRotationMatrix(prev_drone_angles) * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
        float l = d.length();
        laser_position += (d*laser_speed/l);
    } else {
        laser_position = drone_position;
    }
}

void checkForCollisions() {
    
    float r_asteroid = 1;
    float r_laser = 0.2;
    float r_drone = 0.5;
    float r_star = 0.1;
    float r_medkit = 0.5;
    float r_satellite = 0.5;
    
    // laser/drone-asteroid collision
    for (int i=0; i<asteroid_positions.size(); i++) {
        
        glm::vec3 d = asteroid_positions[i].first-laser_position;
        float dist2 = glm::dot(d,d);
        float radiusSum = r_asteroid + r_laser;
        // TODO: improve distance to both l-r lasers
        // TODO: use a tighter hitbox
        if (dist2 <= radiusSum*radiusSum && isLaserActive) {
            double t = glfwGetTime();
            lastTimeLaserHit = t;
            collisionPosition = asteroid_positions[i].first;
            asteroid_positions.erase(asteroid_positions.begin() + i);
        }
        
        d = asteroid_positions[i].first-drone_position;
        dist2 = glm::dot(d,d);
        radiusSum = r_asteroid + r_drone;
        // TODO: improve distance to both l-r lasers
        // TODO: use a tighter hitbox
        if (dist2 <= radiusSum*radiusSum && !isLaserActive) {
            double t = glfwGetTime();
            lastTimeLaserHit = t;
            collisionPosition = asteroid_positions[i].first;
            asteroid_positions.erase(asteroid_positions.begin() + i);
            PLAYER_HEALTH--;
        }
        
    }
    
    // medkit-drone collision
    for (int i=0; i<medkit_positions.size(); i++) {
        glm::vec3 d = medkit_positions[i].first-drone_position;
        float dist2 = glm::dot(d,d);
        float radiusSum = r_drone + r_medkit;
        // TODO: improve distance to both l-r lasers
        // TODO: use a tighter hitbox
        if (dist2 <= radiusSum*radiusSum) {
            PLAYER_HEALTH++;
            medkit_positions.erase(medkit_positions.begin() + i);
        }
    }
    
    // star-drone collision
    for (int i=0; i<star_positions.size(); i++) {
        glm::vec3 d = star_positions[i].first-drone_position;
        float dist2 = glm::dot(d,d);
        float radiusSum = r_drone + r_star;
        // TODO: improve distance to both l-r lasers
        // TODO: use a tighter hitbox
        if (dist2 <= radiusSum*radiusSum) {
            PLAYER_SCORE++;
            star_positions.erase(star_positions.begin() + i);
        }
    }
    
    // star-drone collision
    for (int i=0; i<satellite_positions.size(); i++) {
        glm::vec3 d = satellite_positions[i].first-drone_position;
        float dist2 = glm::dot(d,d);
        float radiusSum = r_drone + r_satellite;
        // TODO: improve distance to both l-r lasers
        // TODO: use a tighter hitbox
        if (dist2 <= radiusSum*radiusSum) {
            PLAYER_HEALTH--;
            satellite_positions.erase(satellite_positions.begin() + i);
        }
    }
    
}

void updateDronePosition() {
    // Engines are working
    if (areEnginesRunning) {
        glm::vec3 d = glm::vec3(getRotationMatrix(current_drone_angles) * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
        float l = d.length();
        drone_position += (d*drone_speed/l);
    }
    // Slowing Down
    double t = glfwGetTime();
    if (!areEnginesRunning && t-lastTimeEnginesStopped<stoppingTime) { // 2
        glm::vec3 d = glm::vec3(getRotationMatrix(current_drone_angles) * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
        float l = d.length();
        float damping = 1 - ((t-lastTimeEnginesStopped)/stoppingTime);
        drone_position += (d*drone_speed*damping/l);
    }
}

//========================================================================================
//========================================================================================

static void char_callback(GLFWwindow *window, unsigned int key)
{
    
    double t = glfwGetTime();
    
    keyToggles[key] = !keyToggles[key];
    switch(key) {
        case 'w': // TODO: add restrictions
            target_drone_angles.x += drone_rotation_speed;
            break;
        case 's':
            target_drone_angles.x -= drone_rotation_speed;
            break;
        case 'd':
            target_drone_angles.y -= drone_rotation_speed;
            break;
        case 'a':
            target_drone_angles.y += drone_rotation_speed;
            break;
        case ' ':
            
            // cant immediately restart engines
            if (t-lastTimeEnginesStopped>stoppingTime) {

                areEnginesRunning = !areEnginesRunning;
                if (!areEnginesRunning) {
                    lastTimeEnginesStopped = t;
                }
                
            }

            break;
        case 'l':
            bool canShootLaser = t-lastTimeLaserShot>laser_reload_time;
            if (canShootLaser) {
                cout << "Shooting!" << endl;
                lastTimeLaserShot = t;
                laser_position = drone_position;
                prev_drone_angles = current_drone_angles;
                isLaserActive = true; // NOT DOING ANYTHING REALLY
            } else {
                cout << "Must reload..." << endl;
                isLaserActive = false; // NOT DOING ANYTHING REALLY
            }
            break;
//        case 'b':
//            // TODO: roll for boost
//            // or when going through hoop/star?
//            break;
    }
}

void stepParticles()
{
    
    // TODO: This can be parallelized!
    
    double t = glfwGetTime();
    bool didTrailDissapate = t-lastTimeEnginesStopped>4;
    
    // smoke
    for(int i = 0; i < (int)smoke_particles.size(); ++i) {
        smoke_particles[i]->step(t, h, grav, keyToggles);
    }
    
    for(int i = 0; i < (int)fire_particles.size(); ++i) {
        fire_particles[i]->step(t, h, grav, keyToggles, drone_position, current_drone_angles, areEnginesRunning, didTrailDissapate); // times["Engine"]
    }
    
    // trail
    for(int i = 0; i < (int)trailing_particles.size(); ++i) {
        trailing_particles[i]->step(t, h, grav, keyToggles, drone_position, current_drone_angles, areEnginesRunning, didTrailDissapate); // TODO: add dese nuts
    }

    // laser
    if (isLaserActive) {
        for(int i = 0; i < (int)laser_particles.size(); ++i) {
            laser_particles[i]->step(t, h, grav, keyToggles);
        }
        
    }
    
    // explosion
    bool isExplosionActive = t < (lastTimeLaserHit + explosion_duration); // currently will do it anytime it has been shot
    if (isExplosionActive) {
        bool keepSpawning = t-lastTimeLaserHit < (explosion_duration-1.0f)/2.0f;
        for(int i = 0; i < (int)explosion_particles.size(); ++i) {
            explosion_particles[i]->step(t, h, grav, keyToggles, keepSpawning);
        }
    }
    
    t += h;
    
    
    // TODO: set t = 0, make individial times for each particle group, so start/stop looks good
    // times["Explosion"] += h;
    
}

static void init()
{
    
    cout << "init()" << endl;
    
    GLSL::checkVersion();
    
    // Set background color
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    
    // Enable z-buffer test
    glEnable(GL_DEPTH_TEST);
    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Enable setting gl_PointSize from vertex shader
    glEnable(GL_PROGRAM_POINT_SIZE);
    // Enable quad creation from sprite
    glEnable(GL_POINT_SPRITE);
    
    //========================================================================================
    //========================================================================================
        
    prog = make_shared<Program>();
    setProgram(prog);
    
    prog_particle = make_shared<Program>();
    setParticleProgram(prog_particle);
    
    times["Engine"] = 0.0f;
    times["Smoke"] = 0.0f;
    times["Trail"] = 0.0f;
    times["Laser"] = 0.0f;
    times["Explosion"] = 0.0f;
    
    // OBJ Files
    initDrone();
    initEarth();
    initSun();
    initAsteroids();
    initSatellite();
    initMedkits();
    initStars();
    
    // Particles
    initFlareParticles();
    initTrailParticles();
    initSmokeParticles();
    initLaserParticles();
    initExplosionParticles();
    
    // Other
    initSkybox();
    getRandomAsteroidPositions();
    getRandomSatellitePositions();
    getRandomMedkitPositions();
    getRandomStarPositions();
    
    //========================================================================================
    //========================================================================================

    GLSL::checkError(GET_FILE_LINE);
    
    cout << "init() completed." << endl;
    
}

void render()
{
    
    // Update time.
    // double t = glfwGetTime();
    
	// Get current frame buffer size.
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	
	// Use the window size for camera.
	glfwGetWindowSize(window, &width, &height);
	camera->setAspect((float)width/(float)height);
	
	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(keyToggles[(unsigned)'c']) {
		glEnable(GL_CULL_FACE);
	} else {
		glDisable(GL_CULL_FACE);
	}
	if(keyToggles[(unsigned)'z']) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
    
    //========================================================================================
    //========================================================================================
    
    auto P = make_shared<MatrixStack>();
    auto MV = make_shared<MatrixStack>();
    
    //========================================================================================
    //========================================================================================
    
    // TODOL Currently drawing skybox before HUD (disabling GL_DEPTH_TEST for better performance)
    P->pushMatrix();
    MV->pushMatrix();
        camera->applyProjectionMatrix(P);
        camera->applyViewMatrix(MV);
        // temporary
        glm::mat4 rot1 = getRotationMatrix(current_drone_angles);
        glm::mat4 E1 = rot1;
        E1[3] = glm::vec4(drone_position, 1.0f);
        MV->multMatrix(glm::inverse(E1));
        // temporary
        drawSkybox(P, MV);
    P->popMatrix();
    MV->popMatrix();
    
    //========================================================================================
    //========================================================================================
    
    drawHUD(P, MV, width, height);
    
    // Apply camera transforms
    P->pushMatrix();
    camera->applyProjectionMatrix(P);
    MV->pushMatrix();
    camera->applyViewMatrix(MV);
    
    //========================================================================================
    //========================================================================================
    
    // Update the current angles
    glm::vec3 delta_angles = (target_drone_angles-current_drone_angles)/30.0f;
    current_drone_angles += delta_angles;
    
    updateDronePosition();
    updateLaserPosition();
    checkForCollisions();

    // Move Camera from Drone
//    glm::vec3 offset = glm::vec3(0,-1,-2);
//    MV->translate(offset);
    
    // Create Matrix for 3rd-Person Camera VView
    glm::mat4 rot = getRotationMatrix(current_drone_angles);
    glm::mat4 E = rot;
    E[3] = glm::vec4(drone_position, 1.0f);
    MV->multMatrix(glm::inverse(E));
    
    // Send the Light Position in Camera Space
    prog->bind();
    glm::vec3 lightPosCam = MV->topMatrix() * glm::vec4(lightPosWorld, 0);
    glUniform3fv(prog->getUniform("lightPos"), 1, glm::value_ptr(lightPosCam));
    prog->unbind();
    
    //========================================================================================
    //========================================================================================
    
    // Draw the Spaceship
    drawDrone(P, MV, width, height, E);
    drawFlare(P, MV, width, height, E);
    drawTrailing(P, MV, width, height);

    // Draw the Objects
    drawEarth(P, MV, width, height);
    drawSun(P, MV, width, height);
    drawAsteroids(P, MV, width, height);
    drawSatellite(P, MV, width, height);
    drawMedkits(P, MV, width, height, false);
    drawStars(P, MV, width, height, false);
    
    // Draw the Particles
    drawSmoke(P, MV, width, height);
    drawLasers(P, MV, width, height);
    drawExplosion(P, MV, width, height); // TODO: duplication and such
    drawGrid(P, MV);

    // Pop stacks
    MV->popMatrix();
    P->popMatrix();
    
    //========================================================================================
    //========================================================================================
	
	GLSL::checkError(GET_FILE_LINE);
}

//========================================================================================
//========================================================================================

int main(int argc, char **argv)
{
    
    cout << "main()" << endl;
    
	if(argc < 2) {
		cout << "Please specify the resource directory." << endl;
		return 0;
	}
	RESOURCE_DIR = argv[1] + string("/");
	
	// Set error callback.
	glfwSetErrorCallback(error_callback);
	// Initialize the library.
	if(!glfwInit()) {
		return -1;
	}
	// Create a windowed mode window and its OpenGL context.
	window = glfwCreateWindow(640, 480, "Cesar Fuentes", NULL, NULL);
	if(!window) {
		glfwTerminate();
		return -1;
	}
	// Make the window's context current.
	glfwMakeContextCurrent(window);
	// Initialize GLEW.
	glewExperimental = true;
	if(glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW" << endl;
		return -1;
	}
	glGetError(); // A bug in glewInit() causes an error that we can safely ignore.
	cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	// Set vsync.
	glfwSwapInterval(1);
	// Set keyboard callback.
	glfwSetKeyCallback(window, key_callback);
	// Set char callback.
	glfwSetCharCallback(window, char_callback);
	// Set cursor position callback.
	glfwSetCursorPosCallback(window, cursor_position_callback);
	// Set mouse button callback.
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	// Initialize scene.
	init();
    
    
	// Loop until the user closes the window.
	while(!glfwWindowShouldClose(window)) {
        
        // Step particles.
        stepParticles();
    
		if(!glfwGetWindowAttrib(window, GLFW_ICONIFIED)) {
			// Render scene.
			render();
			// Swap front and back buffers.
			glfwSwapBuffers(window);
		}
		// Poll for and process events.
		glfwPollEvents();
	}
	// Quit program.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

