#ifndef PLAYGROUND_H
#define PLAYGROUND_H

// Include GLEW
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <stb_image.h>
#include <string>

//some global variables for handling the vertex buffer
GLuint vertexbufferPlayer;
GLuint VertexArrayPlayer;
GLuint vertexbufferPlayer_size;

GLuint vertexbufferOrbital;
GLuint VertexArrayOrbital;
GLuint vertexbufferOrbital_size;

GLuint vertexbufferBackground;
GLuint VertexArrayBackground;
GLuint vertexbufferBackground_size;

//Texture Array
GLuint* texturesID = new GLuint[3];


float degreeOrbitalOrigin;
float degreeOrbitalSelf = 45;
float moveX;
float moveY;
int width = 1920;
int height = 1080;

//program ID of the shaders, required for handling the shaders with OpenGL
GLuint programID;


int main( void ); //<<< main function, called at startup
void updateAnimationLoop(); //<<< updates the animation loop
bool initializeWindow(); //<<< initializes the window using GLFW and GLEW
bool initializeVertexbuffer(); //<<< initializes the vertex buffer array and binds it OpenGL
bool cleanupVertexbuffer(); //<<< frees all recources from the vertex buffer
bool closeWindow(); //<<< Closes the OpenGL window and terminates GLFW
void loadImage(std::string path, int n);
void VAOattributeConfig();
void vertexObjectPlayer();
void vertexObjectOrbital();
void vertexObjectBackground();

#endif
