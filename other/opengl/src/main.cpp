#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <Shader.h>
#include <Camera.h>
#include <Material.h>
#include <LightDirectional.h>
#include <LightPoint.h>
#include <LightSpot.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window,double xpos,double ypos); 
unsigned int LoadImageToGPU(const char* filename,GLint internalFormat,GLenum format,int textureSlot); 

#pragma region Defination
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float lastX,lastY; 
bool firstMouse = 1; 
#pragma endregion

#pragma region Model Data
float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),  
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};
#pragma endregion

Camera camera(glm::vec3(0,0,3.0f),glm::radians(15.0f),glm::radians(180.0f),glm::vec3(0,1.0f,0)); 

#pragma region Light Defination
LightDirectional lightD = LightDirectional(glm::vec3(1.0f,1.0f,-1.0f),
                                          glm::vec3(glm::radians(90.0f),0,0),
                                          glm::vec3(1.0f,1.0f,1.0f)); 


LightPoint lightP0 = LightPoint(glm::vec3(1.0f,0,0),
                                glm::vec3(glm::radians(45.0f),glm::radians(45.0f),0),
                                glm::vec3(1.0f,0,0)); 

LightPoint lightP1 = LightPoint(glm::vec3(0,1.0f,0),
                                glm::vec3(glm::radians(45.0f),glm::radians(45.0f),0),
                                glm::vec3(0,1.0f,0)); 

LightPoint lightP2 = LightPoint(glm::vec3(0,0,1.0f),
                                glm::vec3(glm::radians(45.0f),glm::radians(45.0f),0),
                                glm::vec3(0,0,1.0f)); 

LightPoint lightP3 = LightPoint(glm::vec3(1.0f,1.0f,1.0f),
                                glm::vec3(glm::radians(45.0f),glm::radians(45.0f),0),
                                glm::vec3(1.0f,1.0f,1.0f));        

LightSpot lightS = LightSpot(glm::vec3(0,10.0f,0),
                             glm::vec3(glm::radians(90.0f),0,0),
                             glm::vec3(1.0f,1.0f,1.0f));                                                                                
#pragma endregion

int main()
{   
    #pragma region Init

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "20337228", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // window callback
    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);  //close mouse
    glfwSetCursorPosCallback(window,mouse_callback); 

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST); 
    #pragma endregion

    #pragma region Init Class
    Shader ourShader("./include/vertexShader.vs", "./include/fragmentShader.fs");
    // Material myMaterial(ourShader,
    //                     LoadImageToGPU("./image/container2.png",GL_RGBA,GL_RGBA,Shader::DIFFUSE),
    //                     LoadImageToGPU("./image/container2_specular.png",GL_RGBA,GL_RGBA,Shader::SPECULAR),
    //                     glm::vec3(1.0f,1.0f,1.0f),
    //                     LoadImageToGPU("./image/matrix.jpg",GL_RGB,GL_RGB,Shader::EMMISION),
    //                     32.0f); 

    Material myMaterial(ourShader,
                    LoadImageToGPU("./image/container2.png",GL_RGBA,GL_RGBA,Shader::DIFFUSE),
                    LoadImageToGPU("./image/container2_specular.png",GL_RGBA,GL_RGBA,Shader::SPECULAR),
                    glm::vec3(1.0f,1.0f,1.0f),
                    32.0f); 
    #pragma endregion

    #pragma region Init and Load Model to VAO VBO
    unsigned int VAO; 
    glGenVertexArrays(1, &VAO); 
    glBindVertexArray(VAO);

    unsigned int VBO; 
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);

    // lighting attribute
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); 
    glEnableVertexAttribArray(3); 

    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    #pragma endregion

    #pragma region Init and Load Textures
    // unsigned int TexBufferA;
    // TexBufferA = LoadImageToGPU("./image/container.jpg",GL_RGB,GL_RGB,0); 
    // unsigned int TexBufferB;
    // TexBufferB = LoadImageToGPU("./image/awesomeface.png",GL_RGBA,GL_RGBA,0);
    #pragma endregion

    #pragma region prepare MVP matrices
    glm::mat4 modelMat; 
    glm::mat4 viewMat; 
    glm::mat4 projMat; 
    projMat = glm::perspective(glm::radians(45.0f),(SCR_WIDTH*1.0f)/(SCR_HEIGHT*1.0f),0.1f,100.0f); 
    #pragma endregion

    // Shader loop
    while (!glfwWindowShouldClose(window))
    {  
        //Input 
        processInput(window);

        // Clear Screen
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        viewMat = camera.GetViewMatrix(); 

        for (unsigned int i = 0; i < 10; i++)
        {
            // Set Model matrix
            modelMat = glm::translate(glm::mat4(1.0f),cubePositions[i]); 

            // Set View and Projection Matrices here if you want

            // Set Material -> Texture
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, myMaterial.diffuse);
            glActiveTexture(GL_TEXTURE0 + 1);
            glBindTexture(GL_TEXTURE_2D, myMaterial.specular);
            // Set Material -> Shader
            ourShader.use();
            // Set Material -> Uniform
            // glUniform1i(glGetUniformLocation(ourShader.ID,"ourTexture"),0); 
            // glUniform1i(glGetUniformLocation(ourShader.ID,"ourFace"),1); 

            glUniformMatrix4fv(glGetUniformLocation(ourShader.ID,"modelMat"),1,GL_FALSE,glm::value_ptr(modelMat));
            glUniformMatrix4fv(glGetUniformLocation(ourShader.ID,"viewMat"),1,GL_FALSE,glm::value_ptr(viewMat));
            glUniformMatrix4fv(glGetUniformLocation(ourShader.ID,"projMat"),1,GL_FALSE,glm::value_ptr(projMat));
            glUniform3f(glGetUniformLocation(ourShader.ID,"objColor"),1.0f,1.0f,1.0f); 
            glUniform3f(glGetUniformLocation(ourShader.ID,"ambientColor"),0.1f,0.1f,0.1f); 
            glUniform3f(glGetUniformLocation(ourShader.ID,"cameraPos"), camera.Position.x, camera.Position.y, camera.Position.z); 

            glUniform3f(glGetUniformLocation(ourShader.ID,"lightD.pos"),lightD.position.x,lightD.position.y,lightD.position.z); 
            glUniform3f(glGetUniformLocation(ourShader.ID,"lightD.color"),lightD.color.x,lightD.color.y,lightD.color.z); 
            glUniform3f(glGetUniformLocation(ourShader.ID,"lightD.dirToLight"),lightD.direction.x,lightD.direction.y,lightD.direction.z); 

            glUniform3f(glGetUniformLocation(ourShader.ID,"lightP0.pos"),lightP0.position.x,lightP0.position.y,lightP0.position.z); 
            glUniform3f(glGetUniformLocation(ourShader.ID,"lightP0.color"),lightP0.color.x,lightP0.color.y,lightP0.color.z); 
            glUniform3f(glGetUniformLocation(ourShader.ID,"lightP0.dirToLight"),lightP0.direction.x,lightP0.direction.y,lightP0.direction.z); 
            glUniform1f(glGetUniformLocation(ourShader.ID,"lightP0.constant"),lightP0.constant); 
            glUniform1f(glGetUniformLocation(ourShader.ID,"lightP0.linear"),lightP0.linear);
            glUniform1f(glGetUniformLocation(ourShader.ID,"lightP0.quadratic"),lightP0.quadratic);

            glUniform3f(glGetUniformLocation(ourShader.ID,"lightP1.pos"),lightP1.position.x,lightP1.position.y,lightP1.position.z); 
            glUniform3f(glGetUniformLocation(ourShader.ID,"lightP1.color"),lightP1.color.x,lightP1.color.y,lightP1.color.z); 
            glUniform3f(glGetUniformLocation(ourShader.ID,"lightP1.dirToLight"),lightP1.direction.x,lightP1.direction.y,lightP1.direction.z); 
            glUniform1f(glGetUniformLocation(ourShader.ID,"lightP1.constant"),lightP1.constant); 
            glUniform1f(glGetUniformLocation(ourShader.ID,"lightP1.linear"),lightP1.linear);
            glUniform1f(glGetUniformLocation(ourShader.ID,"lightP1.quadratic"),lightP1.quadratic);

            glUniform3f(glGetUniformLocation(ourShader.ID,"lightP2.pos"),lightP2.position.x,lightP2.position.y,lightP2.position.z); 
            glUniform3f(glGetUniformLocation(ourShader.ID,"lightP2.color"),lightP2.color.x,lightP2.color.y,lightP2.color.z); 
            glUniform3f(glGetUniformLocation(ourShader.ID,"lightP2.dirToLight"),lightP2.direction.x,lightP2.direction.y,lightP2.direction.z); 
            glUniform1f(glGetUniformLocation(ourShader.ID,"lightP2.constant"),lightP2.constant); 
            glUniform1f(glGetUniformLocation(ourShader.ID,"lightP2.linear"),lightP2.linear);
            glUniform1f(glGetUniformLocation(ourShader.ID,"lightP2.quadratic"),lightP2.quadratic);

            glUniform3f(glGetUniformLocation(ourShader.ID,"lightP3.pos"),lightP3.position.x,lightP3.position.y,lightP3.position.z); 
            glUniform3f(glGetUniformLocation(ourShader.ID,"lightP3.color"),lightP3.color.x,lightP3.color.y,lightP3.color.z); 
            glUniform3f(glGetUniformLocation(ourShader.ID,"lightP3.dirToLight"),lightP3.direction.x,lightP3.direction.y,lightP3.direction.z); 
            glUniform1f(glGetUniformLocation(ourShader.ID,"lightP3.constant"),lightP3.constant); 
            glUniform1f(glGetUniformLocation(ourShader.ID,"lightP3.linear"),lightP3.linear);
            glUniform1f(glGetUniformLocation(ourShader.ID,"lightP3.quadratic"),lightP3.quadratic);

            glUniform3f(glGetUniformLocation(ourShader.ID,"lightS.pos"),lightS.position.x,lightS.position.y,lightS.position.z); 
            glUniform3f(glGetUniformLocation(ourShader.ID,"lightS.color"),lightS.color.x,lightS.color.y,lightS.color.z); 
            glUniform3f(glGetUniformLocation(ourShader.ID,"lightS.dirToLight"),lightS.direction.x,lightS.direction.y,lightS.direction.z); 
            glUniform1f(glGetUniformLocation(ourShader.ID,"lightS.constant"),lightS.constant); 
            glUniform1f(glGetUniformLocation(ourShader.ID,"lightS.linear"),lightS.linear);
            glUniform1f(glGetUniformLocation(ourShader.ID,"lightS.quadratic"),lightS.quadratic);
            glUniform1f(glGetUniformLocation(ourShader.ID,"lightS.cosPhyInner"),lightS.cosPhyInner);
            glUniform1f(glGetUniformLocation(ourShader.ID,"lightS.cosPhyOuter"),lightS.cosPhyOuter);

            ourShader.SetUniform3f("material.ambient",myMaterial.ambient); 
            ourShader.SetUniform1i("material.diffuse",Shader::DIFFUSE); 
            ourShader.SetUniform1i("material.specular",Shader::SPECULAR); 
            //ourShader.SetUniform1i("material.emission",Shader::EMMISION); 
            ourShader.SetUniform1f("material.shininess",myMaterial.shininess); 

            // Set Model
            glBindVertexArray(VAO);

            // Drawcall
            glDrawArrays(GL_TRIANGLES,0,36); 
        }

        //Clean up
        glfwSwapBuffers(window);
        glfwPollEvents();
        camera.UpdateCameraPos(); 
    }

    #pragma region Delete and Exit
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
    #pragma endregion
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    // W S
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.speedZ = 1.0f;       
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.speedZ = -1.0f; 
    }
    else 
    {
        camera.speedZ = 0; 
    }

    // A D
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.speedX = 1.0f;       
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.speedX = -1.0f; 
    }
    else 
    {
        camera.speedX = 0; 
    }

    // Q E
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera.speedY = 1.0f;       
    }
    else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera.speedY = -1.0f; 
    }
    else 
    {
        camera.speedY = 0; 
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window,double xpos,double ypos)
{
    if(firstMouse)
    {
        lastX = xpos; 
        lastY = ypos; 
        firstMouse = 0; 
    }

    float deltaX,deltaY; 
    deltaX = xpos - lastX; 
    deltaY = ypos - lastY; 

    lastX = xpos; 
    lastY = ypos; 

    camera.ProcessMouseMovement(deltaX,deltaY); 
}

unsigned int LoadImageToGPU(const char* filename,GLint internalFormat,GLenum format,int textureSlot)
{
    unsigned int TexBuffer; 
    glGenTextures(1,&TexBuffer); 
    glActiveTexture(GL_TEXTURE0 + textureSlot); 
    glBindTexture(GL_TEXTURE_2D,TexBuffer); 

    int width,height,nrChannel; 
    stbi_set_flip_vertically_on_load(true); 
    unsigned char *data = stbi_load(filename,&width,&height,&nrChannel,0); 
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,internalFormat,width,height,0,format,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D); 
    }
    else
    {
        printf("load image failed.\n"); 
    }
    stbi_image_free(data); 
    return TexBuffer; 
} 
