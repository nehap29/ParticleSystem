#include <GLFW/glfw3.h>
extern GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"

glm::mat4 VM;
glm::mat4 ProjectionMatrix;
float lifeTime = 10.0f;
float c1 = 0.15;
float c2 = 0.5;
int red = 255;
int blue = 255;
int green = 255;

float getLifeTime(){
    return lifeTime;
}

float getConst1(){
    return c1;
}

float getConst2(){
    return c2;
}

int getRed(){
    return red;
}

int getBlue(){
    return blue;
}

int getGreen(){
    return green;
}

glm::mat4 getVM(){
    return VM;
}

glm::mat4 getProjectionMatrix(){
    return ProjectionMatrix;
}


glm::vec3 position = glm::vec3( 0, 10, 60 );
float hAngle = 3.14f;
float vAngle = 0.0f;
float initialFoV = 45.0f;

float speed = 10.0f;
float mouseSpeed = 0.005f;


void computeMatricesFromInputs(){

	static double lastTime = glfwGetTime();

	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

    //mouse pos
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	//reset mouse pos
	glfwSetCursorPos(window, 1024/2, 768/2);

	//calc new orientation
	hAngle += mouseSpeed * float(1024/2 - xpos );
	vAngle   += mouseSpeed * float( 768/2 - ypos );

	//Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(vAngle) * sin(hAngle),
		sin(vAngle),
		cos(vAngle) * cos(hAngle)
	);
	
	glm::vec3 right = glm::vec3(
		sin(hAngle - 3.14f/2.0f),
		0,
		cos(hAngle - 3.14f/2.0f)
	);
	
	glm::vec3 up = glm::cross( right, direction );

	//forward
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	//backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	//right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	//left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}
    //inc lifetime
    if (glfwGetKey( window, GLFW_KEY_1 ) == GLFW_PRESS){
        lifeTime = lifeTime + 2;
    }
    //dec lifetime
    if (glfwGetKey( window, GLFW_KEY_2 ) == GLFW_PRESS){
        lifeTime = lifeTime - 2;
    }
    //inc const1
    if (glfwGetKey( window, GLFW_KEY_3 ) == GLFW_PRESS){
        c1 = c1 + 0.01;
    }
    //dec const1
    if (glfwGetKey( window, GLFW_KEY_4 ) == GLFW_PRESS){
        c1 = c1 - 0.01;
    }
    //inc const2
    if (glfwGetKey( window, GLFW_KEY_5 ) == GLFW_PRESS){
        c2 = c2 + 0.01;
    }
    //dec const2
    if (glfwGetKey( window, GLFW_KEY_6 ) == GLFW_PRESS){
        c2 = c2 - 0.01;
    }
    //inc red
    if (glfwGetKey( window, GLFW_KEY_R ) == GLFW_PRESS){
        if(red <= 245){
            red = red + 10;
        }
    }
    //dec red
    if (glfwGetKey( window, GLFW_KEY_B ) == GLFW_PRESS){
        if(blue <= 245){
            blue = blue + 10;
        }
    }
    //inc green
    if (glfwGetKey( window, GLFW_KEY_G ) == GLFW_PRESS){
        if(green <= 245){
            green = green + 10;
        }
    }
    //dec green
    if (glfwGetKey( window, GLFW_KEY_E ) == GLFW_PRESS){
        if(red >= 10){
            red = red - 10;
        }
    }
    //inc blue
    if (glfwGetKey( window, GLFW_KEY_V ) == GLFW_PRESS){
        if(blue >= 10){
            blue = blue - 20;
        }
    }
    //dec blue
    if (glfwGetKey( window, GLFW_KEY_F ) == GLFW_PRESS){
        if(green >= 10){
            green = green - 10;
        }
    }
    
    
    


    float FoV = initialFoV;

	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	VM = glm::lookAt(position,position+direction,up);
	lastTime = currentTime;
}
