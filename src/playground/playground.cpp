#include "playground.h"
#include <iostream>
#include <Windows.h>
#include "stb_image.h"
#include <string>


// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

// Include shader
#include <common/shader.hpp>


int main(void)
{

    //Initialize window
    bool windowInitialized = initializeWindow();
    if (!windowInitialized) return -1;

    //Initialize vertex buffer
    bool vertexbufferInitialized = initializeVertexbuffer();
    if (!vertexbufferInitialized) return -1;

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

    //Generate Textures
    glGenTextures(2, texturesID);

    loadImage("./assets/Swerg.png", 0);
    loadImage("./assets/Axt.png", 1);
    loadImage("./assets/Background.png", 2);

    //start animation loop until escape key is pressed

    do {
        if (glfwGetKey(window, GLFW_KEY_D) == true) {
            moveX += 0.001;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == true) {
            moveX -= 0.001;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == true) {
            moveY += 0.001;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == true) {
            moveY -= 0.001;
        }
        degreeOrbitalOrigin += 0.05;
        if (degreeOrbitalOrigin > 360)
            degreeOrbitalOrigin = (int)degreeOrbitalOrigin % 360;

       
        updateAnimationLoop();

    }
    // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);

        //Cleanup and close window
        //glDeleteTextures(1, &texture);
        cleanupVertexbuffer();
        glDeleteProgram(programID);
	    closeWindow();
  
	    return 0;
    }

void updateAnimationLoop()
{
  initializeVertexbuffer();

  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT);

  // Use our shader
  glUseProgram(programID);

  // Bind attributs at BACKGROUNDBUFFER
  glBindBuffer(GL_ARRAY_BUFFER, vertexbufferBackground);
  glBindTexture(GL_TEXTURE_2D, texturesID[2]);
  glBindVertexArray(VertexArrayBackground);
  //Draw the triangle !
  glDrawArrays(GL_TRIANGLES, 0, vertexbufferBackground_size);
  //Unbind all attributs
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDisableVertexAttribArray(0);


  // Bind attributs at PLAYERBUFFER
  glBindBuffer(GL_ARRAY_BUFFER, vertexbufferPlayer);
  glBindTexture(GL_TEXTURE_2D, texturesID[0]);
  glBindVertexArray(VertexArrayPlayer);
  //Draw the triangle !
  glDrawArrays(GL_TRIANGLES, 0, vertexbufferPlayer_size);
  //Unbind all attributs
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDisableVertexAttribArray(0);


  // Bind attributs at ORBITALBUFFER
  glBindBuffer(GL_ARRAY_BUFFER, vertexbufferOrbital);
  glBindTexture(GL_TEXTURE_2D, texturesID[1]);
  glBindVertexArray(VertexArrayOrbital);
  //Draw the triangle !
  glDrawArrays(GL_TRIANGLES, 0, vertexbufferOrbital_size);
  //Unbind all attributs
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDisableVertexAttribArray(0);



  // Swap buffers
  glfwSwapBuffers(window);
  glfwPollEvents();
}


//Rotation Matrix
glm::mat2 calculateRotationM(float degree) {
    float radiant = degree * (3.14 / 180);
    glm::mat2 rotationMatrix = glm::mat2(cos(radiant), -sin(radiant),
        sin(radiant), cos(radiant));

    return rotationMatrix;
}


bool initializeVertexbuffer()
{

    vertexObjectPlayer();
    vertexObjectOrbital();
    vertexObjectBackground();
 

  return true;
}

bool cleanupVertexbuffer()
{
  // Cleanup VBO
  glDeleteBuffers(1, &vertexbufferPlayer);
  glDeleteVertexArrays(1, &VertexArrayPlayer);

  glDeleteBuffers(1, &vertexbufferOrbital);
  glDeleteVertexArrays(1, &VertexArrayOrbital);

  glDeleteBuffers(1, &vertexbufferBackground);
  glDeleteVertexArrays(1, &VertexArrayBackground);
  return true;
}

bool closeWindow()
{
  glfwTerminate();
  return true;
}

void loadImage(std::string path, int n) {
    const char* cstr = path.c_str();
    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;
    unsigned char* data = stbi_load(cstr, &width, &height, &channels, 4);
    if (data == nullptr) {
        std::cout << "No image found!" << '\n';
    }

    //bind texture to texture slot
    switch (n) {
    case 0: {

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texturesID[n]);
        break;
    }
    case 1: {

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texturesID[n]);
        break;
    }
    case 2: {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texturesID[n]);
        break;
    }
    }

    //Texture config
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    
    switch (n) {
    case 0: {
        glUseProgram(programID);
        glUniform1i(glGetUniformLocation(programID, "texture"), 0);
        break;
    }
    case 1: {
        glUseProgram(programID);
        glUniform1i(glGetUniformLocation(programID, "texture"), 1);
        break;
    }
    case 2: {
        glUseProgram(programID);
        glUniform1i(glGetUniformLocation(programID, "texture"), 2);
        break;

    }
    }
}


    void vertexObjectPlayer() {

        glGenVertexArrays(1, &VertexArrayPlayer);
        glBindVertexArray(VertexArrayPlayer);

        vertexbufferPlayer_size = 6;

        glm::vec2 triangleVertice1 = glm::vec2(-((2.0f / width) * 64), ((2.0f / height) * 64)); //oben links
        glm::vec2 triangleVertice2 = glm::vec2(((2.0f / width) * 64), ((2.0f / height) * 64)); //oben rechts
        glm::vec2 triangleVertice3 = glm::vec2(((2.0f / width) * 64), -((2.0f / height) * 64)); //unten rechts
        glm::vec2 triangleVertice4 = glm::vec2(-((2.0f / width) * 64), -((2.0f / height) * 64)); //unten links

           
        //Input player movement
        glm::vec2 inputXT = glm::vec2(moveX, 0.0f);
        glm::vec2 inputYT = glm::vec2(0.0f, moveY);

        triangleVertice1 = triangleVertice1 + inputXT + inputYT;
        triangleVertice2 = triangleVertice2 + inputXT + inputYT;
        triangleVertice3 = triangleVertice3 + inputXT + inputYT;
        triangleVertice4 = triangleVertice4 + inputXT + inputYT;
      

        static GLfloat g_vertex_buffer_data[48];

        g_vertex_buffer_data[0] = triangleVertice1[0];
        g_vertex_buffer_data[1] = triangleVertice1[1];
        g_vertex_buffer_data[2] = 0.0f;
        //Farben
        g_vertex_buffer_data[3] = 1.0f;
        g_vertex_buffer_data[4] = 1.0f;
        g_vertex_buffer_data[5] = 1.0f;
        //Bildkoordianten
        g_vertex_buffer_data[6] = 0.0f;
        g_vertex_buffer_data[7] = 1.0f;

        g_vertex_buffer_data[8] = triangleVertice2[0];
        g_vertex_buffer_data[9] = triangleVertice2[1];
        g_vertex_buffer_data[10] = 0.0f;
        //Farben
        g_vertex_buffer_data[11] = 1.0f;
        g_vertex_buffer_data[12] = 1.0f;
        g_vertex_buffer_data[13] = 1.0f;
        //Bildkoordianten
        g_vertex_buffer_data[14] = 1.0f;
        g_vertex_buffer_data[15] = 1.0f;

        g_vertex_buffer_data[16] = triangleVertice3[0];
        g_vertex_buffer_data[17] = triangleVertice3[1];
        g_vertex_buffer_data[18] = 0.0f;
        //Farben
        g_vertex_buffer_data[19] = 1.0f;
        g_vertex_buffer_data[20] = 1.0f;
        g_vertex_buffer_data[21] = 1.0f;
        //Bildkoordianten
        g_vertex_buffer_data[22] = 1.0f;
        g_vertex_buffer_data[23] = 0.0f;

        g_vertex_buffer_data[24] = triangleVertice4[0];
        g_vertex_buffer_data[25] = triangleVertice4[1];
        g_vertex_buffer_data[26] = 0.0f;
        //Farben
        g_vertex_buffer_data[27] = 1.0f;
        g_vertex_buffer_data[28] = 1.0f;
        g_vertex_buffer_data[29] = 1.0f;
        //Bildkoordianten
        g_vertex_buffer_data[30] = 0.0f;
        g_vertex_buffer_data[31] = 0.0f;

        g_vertex_buffer_data[32] = triangleVertice1[0];
        g_vertex_buffer_data[33] = triangleVertice1[1];
        g_vertex_buffer_data[34] = 0.0f;
        //Farben
        g_vertex_buffer_data[35] = 1.0f;
        g_vertex_buffer_data[36] = 1.0f;
        g_vertex_buffer_data[37] = 1.0f;
        //Bildkoordianten
        g_vertex_buffer_data[38] = 0.0f;
        g_vertex_buffer_data[39] = 1.0f;

        g_vertex_buffer_data[40] = triangleVertice3[0];
        g_vertex_buffer_data[41] = triangleVertice3[1];
        g_vertex_buffer_data[42] = 0.0f;
        //Farben
        g_vertex_buffer_data[43] = 1.0f;
        g_vertex_buffer_data[44] = 1.0f;
        g_vertex_buffer_data[45] = 1.0f;
        //Bildkoordianten
        g_vertex_buffer_data[46] = 1.0f;
        g_vertex_buffer_data[47] = 0.0f;


        glGenBuffers(1, &vertexbufferPlayer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbufferPlayer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        VAOattributeConfig();

        //Unbind vertexbuffer 
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        //Unbind vertexarray
        glBindVertexArray(0);

    };



    void vertexObjectOrbital() {

        glGenVertexArrays(1, &VertexArrayOrbital);
        glBindVertexArray(VertexArrayOrbital);

        vertexbufferOrbital_size = 6;

        //Rotation Matrix
        glm::mat2 rotationOrigin = calculateRotationM(degreeOrbitalOrigin);
        glm::mat2 myR1 = calculateRotationM(degreeOrbitalSelf);


        //+++++Orbital+++++//
      
        glm::vec2 orbitalVertice1 = glm::vec2(-0.05f, 0.05f); //oben links
        glm::vec2 orbitalVertice2 = glm::vec2(0.05f, 0.05f); //rechts
        glm::vec2 orbitalVertice3 = glm::vec2(0.05f, -0.05f);//unten links
        glm::vec2 orbitalVertice4 = glm::vec2(-0.05f, -0.05f); 
        

        //Orbital distance
        glm::vec2 orbitalDistanceT = glm::vec2(0.2f, 0.0f);

        //Orbital position and rotation
        orbitalVertice1 = myR1 * orbitalVertice1;
        orbitalVertice2 = myR1 * orbitalVertice2;
        orbitalVertice3 = myR1 * orbitalVertice3;
        orbitalVertice4 = myR1 * orbitalVertice4;
        
        orbitalVertice1 = orbitalVertice1 + orbitalDistanceT;
        orbitalVertice2 = orbitalVertice2 + orbitalDistanceT;
        orbitalVertice3 = orbitalVertice3 + orbitalDistanceT;
        orbitalVertice4 = orbitalVertice4 + orbitalDistanceT;
        
        orbitalVertice1 = rotationOrigin * orbitalVertice1;
        orbitalVertice2 = rotationOrigin * orbitalVertice2;
        orbitalVertice3 = rotationOrigin * orbitalVertice3;
        orbitalVertice4 = rotationOrigin * orbitalVertice4;

        glm::vec2 inputXT = glm::vec2(moveX, 0.0f);
        glm::vec2 inputYT = glm::vec2(0.0f, moveY);
        
        orbitalVertice1 = orbitalVertice1 + inputXT + inputYT;
        orbitalVertice2 = orbitalVertice2 + inputXT + inputYT;
        orbitalVertice3 = orbitalVertice3 + inputXT + inputYT;
        orbitalVertice4 = orbitalVertice4 + inputXT + inputYT;



        static GLfloat g_vertex_buffer_data[49];

        g_vertex_buffer_data[0] = orbitalVertice1[0];
        g_vertex_buffer_data[1] = orbitalVertice1[1];
        g_vertex_buffer_data[2] = 0.0f;
        //Farben
        g_vertex_buffer_data[3] = 1.0f;
        g_vertex_buffer_data[4] = 1.0f;
        g_vertex_buffer_data[5] = 1.0f;
        //Bildkoordianten
        g_vertex_buffer_data[6] = 0.0f;
        g_vertex_buffer_data[7] = 1.0f;

        g_vertex_buffer_data[8] = orbitalVertice2[0];
        g_vertex_buffer_data[9] = orbitalVertice2[1];
        g_vertex_buffer_data[10] = 0.0f;
        //Farben
        g_vertex_buffer_data[11] = 1.0f;
        g_vertex_buffer_data[12] = 1.0f;
        g_vertex_buffer_data[13] = 1.0f;
        //Bildkoordianten
        g_vertex_buffer_data[14] = 1.0f;
        g_vertex_buffer_data[15] = 1.0f;

        g_vertex_buffer_data[16] = orbitalVertice3[0];
        g_vertex_buffer_data[17] = orbitalVertice3[1];
        g_vertex_buffer_data[18] = 0.0f;
        //Farben
        g_vertex_buffer_data[19] = 1.0f;
        g_vertex_buffer_data[20] = 1.0f;
        g_vertex_buffer_data[21] = 1.0f;
        //Bildkoordianten
        g_vertex_buffer_data[22] = 1.0f;
        g_vertex_buffer_data[23] = 0.0f;

        g_vertex_buffer_data[24] = orbitalVertice4[0];
        g_vertex_buffer_data[25] = orbitalVertice4[1];
        g_vertex_buffer_data[26] = 0.0f;
        //Farben
        g_vertex_buffer_data[27] = 1.0f;
        g_vertex_buffer_data[28] = 1.0f;
        g_vertex_buffer_data[29] = 1.0f;
        //Bildkoordianten
        g_vertex_buffer_data[30] = 0.0f;
        g_vertex_buffer_data[31] = 0.0f;

        g_vertex_buffer_data[32] = orbitalVertice1[0];
        g_vertex_buffer_data[33] = orbitalVertice1[1];
        g_vertex_buffer_data[34] = 0.0f;
        //Farben
        g_vertex_buffer_data[35] = 1.0f;
        g_vertex_buffer_data[36] = 1.0f;
        g_vertex_buffer_data[37] = 1.0f;
        //Bildkoordianten
        g_vertex_buffer_data[38] = 0.0f;
        g_vertex_buffer_data[39] = 1.0f;

        g_vertex_buffer_data[40] = orbitalVertice3[0];
        g_vertex_buffer_data[41] = orbitalVertice3[1];
        g_vertex_buffer_data[42] = 0.0f;
        //Farben
        g_vertex_buffer_data[43] = 1.0f;
        g_vertex_buffer_data[44] = 1.0f;
        g_vertex_buffer_data[45] = 1.0f;
        //Bildkoordianten
        g_vertex_buffer_data[46] = 1.0f;
        g_vertex_buffer_data[47] = 0.0f;

        

        glGenBuffers(1, &vertexbufferOrbital);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbufferOrbital);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        VAOattributeConfig();

        //Unbind vertexbuffer 
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        //Unbind vertexarray
        glBindVertexArray(0);



    };


    void vertexObjectBackground() {

        glGenVertexArrays(1, &VertexArrayBackground);
        glBindVertexArray(VertexArrayBackground);

        vertexbufferBackground_size = 6;

     
        glm::vec2 backgroundVertice1 = glm::vec2(-1.0f, 1.0f); //oben links
        glm::vec2 backgroundVertice2 = glm::vec2(1.0f, 1.0f);  //oben rechts
        glm::vec2 backgroundVertice3 = glm::vec2(1.0f, -1.0f); //unten rechts
        glm::vec2 backgroundVertice4 = glm::vec2(-1.0f, -1.0f); //unten links



        static GLfloat g_vertex_buffer_data[49];

        g_vertex_buffer_data[0] = backgroundVertice1[0];
        g_vertex_buffer_data[1] = backgroundVertice1[1];
        g_vertex_buffer_data[2] = 0.0f;
        //Farben
        g_vertex_buffer_data[3] = 1.0f;
        g_vertex_buffer_data[4] = 1.0f;
        g_vertex_buffer_data[5] = 1.0f;
        //Bildkoordianten
        g_vertex_buffer_data[6] = 0.0f;
        g_vertex_buffer_data[7] = 1.0f;

        g_vertex_buffer_data[8] = backgroundVertice2[0];
        g_vertex_buffer_data[9] = backgroundVertice2[1];
        g_vertex_buffer_data[10] = 0.0f;
        //Farben
        g_vertex_buffer_data[11] = 1.0f;
        g_vertex_buffer_data[12] = 1.0f;
        g_vertex_buffer_data[13] = 1.0f;
        //Bildkoordianten
        g_vertex_buffer_data[14] = 1.0f;
        g_vertex_buffer_data[15] = 1.0f;

        g_vertex_buffer_data[16] = backgroundVertice3[0];
        g_vertex_buffer_data[17] = backgroundVertice3[1];
        g_vertex_buffer_data[18] = 0.0f;
        //Farben
        g_vertex_buffer_data[19] = 1.0f;
        g_vertex_buffer_data[20] = 1.0f;
        g_vertex_buffer_data[21] = 1.0f;
        //Bildkoordianten
        g_vertex_buffer_data[22] = 1.0f;
        g_vertex_buffer_data[23] = 0.0f;

        g_vertex_buffer_data[24] = backgroundVertice4[0];
        g_vertex_buffer_data[25] = backgroundVertice4[1];
        g_vertex_buffer_data[26] = 0.0f;
        //Farben
        g_vertex_buffer_data[27] = 1.0f;
        g_vertex_buffer_data[28] = 1.0f;
        g_vertex_buffer_data[29] = 1.0f;
        //Bildkoordianten
        g_vertex_buffer_data[30] = 0.0f;
        g_vertex_buffer_data[31] = 0.0f;

        g_vertex_buffer_data[32] = backgroundVertice1[0];
        g_vertex_buffer_data[33] = backgroundVertice1[1];
        g_vertex_buffer_data[34] = 0.0f;
        //Farben
        g_vertex_buffer_data[35] = 1.0f;
        g_vertex_buffer_data[36] = 1.0f;
        g_vertex_buffer_data[37] = 1.0f;
        //Bildkoordianten
        g_vertex_buffer_data[38] = 0.0f;
        g_vertex_buffer_data[39] = 1.0f;

        g_vertex_buffer_data[40] = backgroundVertice3[0];
        g_vertex_buffer_data[41] = backgroundVertice3[1];
        g_vertex_buffer_data[42] = 0.0f;
        //Farben
        g_vertex_buffer_data[43] = 1.0f;
        g_vertex_buffer_data[44] = 1.0f;
        g_vertex_buffer_data[45] = 1.0f;
        //Bildkoordianten
        g_vertex_buffer_data[46] = 1.0f;
        g_vertex_buffer_data[47] = 0.0f;



        glGenBuffers(1, &vertexbufferBackground);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbufferBackground);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        VAOattributeConfig();

        //Unbind vertexbuffer 
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        //Unbind vertexarray
        glBindVertexArray(0);



    };

void VAOattributeConfig() {
    // Vertex position attribut configuration
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        8 * sizeof(float),    // stride
        (void*)0            // array buffer offset
    );

    // Vertex color attribut configuration
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,                                    // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                                    // size
        GL_FLOAT,                             // type
        GL_FALSE,                             // normalized?
        8 * sizeof(float),                    // stride
        (void*)(3 * sizeof(float))            // array buffer offset
    );

    // Vertex texture attribut configuration
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,                                    // attribute 0. No particular reason for 0, but must match the layout in the shader.
        2,                                    // size
        GL_FLOAT,                             // type
        GL_FALSE,                             // normalized?
        8 * sizeof(float),                    // stride
        (void*)(6 * sizeof(float))            // array buffer offset
    );
}


bool initializeWindow()
{
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return false;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return false;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Background
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    return true;
}

