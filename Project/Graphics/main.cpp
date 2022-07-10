#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <deque>
#include <vector>
#include <algorithm>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
//#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
using namespace glm;

#include "shader.hpp"
#include "texture.hpp"
#include "controls.hpp"
#include "particle.cpp"

#define GL_SILENCE_DEPRECATION

GLFWwindow* window;

const int particleMax = 100000;
std::deque<Particle*> allParticles(particleMax);

int main( void )
{

    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow( 1024, 768, "Hurricane", NULL, NULL);

    glfwMakeContextCurrent(window);
    
    glewExperimental = true;
    glewInit();

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);
    
    //background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    //Globe globe(0,0,0,40);

    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LESS);
    
    GLuint vertexArray;
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
    
    GLuint program = LoadShaders( "/Users/nehapatil/Desktop/Graphics/Particle.vertexshader", "/Users/nehapatil/Desktop/Graphics/Particle.fragmentshader" );
    
    //vertex shader
    GLuint cameraRLoc  = glGetUniformLocation(program, "cameraRLoc");
    GLuint cameraUpLoc  = glGetUniformLocation(program, "cameraUpLoc");
    GLuint viewProjMatrix = glGetUniformLocation(program, "VP");
    
    //fragment shader
    GLuint fragShader  = glGetUniformLocation(program, "textureSampler");
    
    
    static GLfloat* particleSize = new GLfloat[particleMax * 4];
    static GLubyte* colourInfo         = new GLubyte[particleMax * 4];
    
    for(int i=0; i<particleMax; i++){
        allParticles[i] = new Particle();
        allParticles[i]->lifeTime = -1.0f;
        allParticles[i]->theta = 0;
        //allParticles[i]->bounced = false;

    }
    
    GLuint Texture = loadDDS("/Users/nehapatil/Desktop/Graphics/particleNeha.DDS");

    static const GLfloat g_vertex_buffer_data[] = {-0.5f, -0.5f, 0.0f,
                                                    0.5f, -0.5f, 0.0f,
                                                   -0.5f,  0.5f, 0.0f,
                                                    0.5f,  0.5f, 0.0f,};
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    

    GLuint posBuffer;
    glGenBuffers(1, &posBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
    glBufferData(GL_ARRAY_BUFFER, particleMax * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
    
    GLuint colourBuffer;
    glGenBuffers(1, &colourBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
    glBufferData(GL_ARRAY_BUFFER, particleMax * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
    
    Particle* dataPoint = allParticles[1];
    
    double average_framerate = 0.0f;
    double frameCount = 0;
    
    
    double lastTime = glfwGetTime();
    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        double currentTime = glfwGetTime();
        double timeStamp = currentTime - lastTime;
        //std::cout << timeStamp <<","<<std::endl;
        lastTime = currentTime;
        
        average_framerate += timeStamp;
        frameCount ++;
        computeMatricesFromInputs();
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getVM();
        
        glm::vec3 CameraPosition(glm::inverse(ViewMatrix)[3]);
        
        glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
    
        int newparticles = (int)(timeStamp*10000.0);
        if (newparticles > (int)(0.016f*10000.0))
            newparticles = (int)(0.016f*10000.0);
        
        for(int i=0; i<newparticles; i++){
            Particle* current = allParticles.front();
            if(current->lifeTime > 0){
                break;
            }
            allParticles.pop_front();
            current->lifeTime = getLifeTime();
            current->theta = (float)(rand()%1000000)/10000.0f;
            //current->bounced = false;
            current->pos = glm::vec3(0,0,0);

            float spread = 1.5f;
            glm::vec3 mainVelocity = glm::vec3(0.0f, 10.0f, 0.0f);
            glm::vec3 randVelocity = glm::vec3(0.05f,(rand()%2000 - 1000.0f)/1000.0f,0.05f);
            
            current->velocity = mainVelocity + randVelocity*spread;
            
            current->r = (char)getRed();
            current->g = (char)getGreen();
            current->b = (char)getBlue();
            current->a = (rand() % 256) / 3;
            
            /*if(getRed() + getGreen() + getBlue() <= 200){
                //background
                glClearColor(255.0f, 255.0f, 255.0f, 1.0f);
            }*/
            
            
            current->size = (rand()%1000)/2000.0f + 0.1f;
            
            allParticles.push_back(current);
            
        }

        //std::cout<<(dataPoint->pos[0])<<","<<(dataPoint->pos[1])<<","<<(dataPoint->pos[2])<<std::endl;
        int particlesNum = 0;
        for(int i=0; i<particleMax; i++){
            Particle* p = allParticles[i];
            
            if(p->lifeTime > 0.0f){

                p->lifeTime -= timeStamp;
                p->theta += timeStamp;
                if (p->lifeTime > 0.0f){
/*                    if (globe <= *p && !p->bounced){
                        p->velocity = glm::vec3(0,-9.81,0);
                        p->bounced = true;
                        p->lifeTime = 2.0f;
                        
                    }
                    else if(p->bounced){
                        p->velocity += glm::vec3(0,-9.81,0);
                    }
                    else{*/
                    p->velocity = glm::vec3(getConst2() * exp(getConst1()*p->theta)* -sin(2 * p->theta) * p->theta,
                                           getConst2() * exp(getConst1()*p->theta),
                                           getConst2() * exp(getConst1()*p->theta)* cos(2 * p->theta) * p->theta);
                    
                    p->pos += p->velocity * (float)timeStamp;
                    
                    particleSize[4*particlesNum+0] = p->pos.x;
                    particleSize[4*particlesNum+1] = p->pos.y;
                    particleSize[4*particlesNum+2] = p->pos.z;
                    
                    particleSize[4*particlesNum+3] = p->size;
                    
                    colourInfo[4*particlesNum+0] = p->r;
                    colourInfo[4*particlesNum+1] = p->g;
                    colourInfo[4*particlesNum+2] = p->b;
                    colourInfo[4*particlesNum+3] = p->a;
                    
                }
                particlesNum++;
            }
        }
        
        
        glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
        glBufferData(GL_ARRAY_BUFFER, particleMax * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, particlesNum * sizeof(GLfloat) * 4, particleSize);
        
        glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
        glBufferData(GL_ARRAY_BUFFER, particleMax * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, particlesNum * sizeof(GLubyte) * 4, colourInfo);
        
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUseProgram(program);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glUniform1i(fragShader, 0);
        
        glUniform3f(cameraRLoc, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
        glUniform3f(cameraUpLoc   , ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
        
        glUniformMatrix4fv(viewProjMatrix, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);
        
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
        
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
        glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,0,(void*)0);
        
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
        glVertexAttribPointer(2,4,GL_UNSIGNED_BYTE,GL_TRUE,0,(void*)0);

        glVertexAttribDivisor(0, 0);
        glVertexAttribDivisor(1, 1);
        glVertexAttribDivisor(2, 1);
        
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particlesNum);
        
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );
    
    std::cout << average_framerate / frameCount << std::endl;
    
    
    delete[] particleSize;
    
    glDeleteBuffers(1, &colourBuffer);
    glDeleteBuffers(1, &posBuffer);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteProgram(program);
    glDeleteTextures(1, &Texture);
    glDeleteVertexArrays(1, &vertexArray);
    
    glfwTerminate();
    
    for(auto p:allParticles){
        delete p;
    }
    return 0;
}

