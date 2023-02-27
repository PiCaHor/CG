#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <gl/GLU.h>
#include <gl/GL.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <map>
#include "stb_image.h"
#include "shade.h"
#include "camera.h"
#include "shader.h"
#include "utils.h"
#include "launcher.h"
#include "image-loader.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void updateCameraRotation(GLFWwindow* window);
void processInput(GLFWwindow* window, int* launch_meteor, int* launch_snow);
unsigned int loadCubemap(std::vector<std::string> faces);
GLuint* getAttachmentTexture(GLuint num);

float SCREEN_W = 1920.0f;
float SCREEN_H = 1080.0f;
float FOV = 65.0f;
float SENSITIVITY = 0.1f;

const float NEAR_CLIP = 1.0f;
const float FAR_CLIP = 1000.0f;

double lastXPos = SCREEN_W / 2, lastYPos = SCREEN_H / 2;
double yaw = 0, pitch = 0, xPos, yPos;
unsigned int Launcher::particlesCount = 0;
unsigned int Launcher::lightsCount = 0;

Camera* camera;
glm::highp_mat4 projection;
std::map<int, bool> heldKeys;

Launcher launcher;

#pragma region vertex_data
float skyboxVertices[] = {
    // positions          
    -1000.0f,  1000.0f, -1000.0f,
    -1000.0f, -1000.0f, -1000.0f,
     1000.0f, -1000.0f, -1000.0f,
     1000.0f, -1000.0f, -1000.0f,
     1000.0f,  1000.0f, -1000.0f,
    -1000.0f,  1000.0f, -1000.0f,

    -1000.0f, -1000.0f,  1000.0f,
    -1000.0f, -1000.0f, -1000.0f,
    -1000.0f,  1000.0f, -1000.0f,
    -1000.0f,  1000.0f, -1000.0f,
    -1000.0f,  1000.0f,  1000.0f,
    -1000.0f, -1000.0f,  1000.0f,

     1000.0f, -1000.0f, -1000.0f,
     1000.0f, -1000.0f,  1000.0f,
     1000.0f,  1000.0f,  1000.0f,
     1000.0f,  1000.0f,  1000.0f,
     1000.0f,  1000.0f, -1000.0f,
     1000.0f, -1000.0f, -1000.0f,

    -1000.0f, -1000.0f,  1000.0f,
    -1000.0f,  1000.0f,  1000.0f,
     1000.0f,  1000.0f,  1000.0f,
     1000.0f,  1000.0f,  1000.0f,
     1000.0f, -1000.0f,  1000.0f,
    -1000.0f, -1000.0f,  1000.0f,

    -1000.0f,  1000.0f, -1000.0f,
     1000.0f,  1000.0f, -1000.0f,
     1000.0f,  1000.0f,  1000.0f,
     1000.0f,  1000.0f,  1000.0f,
    -1000.0f,  1000.0f,  1000.0f,
    -1000.0f,  1000.0f, -1000.0f,

    -1000.0f, -1000.0f, -1000.0f,
    -1000.0f, -1000.0f,  1000.0f,
     1000.0f, -1000.0f, -1000.0f,
     1000.0f, -1000.0f, -1000.0f,
    -1000.0f, -1000.0f,  1000.0f,
     1000.0f, -1000.0f,  1000.0f
};
#pragma endregion

int main()
{
    srand(time(NULL));

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_MAXIMIZED, true);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(SCREEN_W, SCREEN_H, "Fireworks", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, key_callback);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    // Camera
    camera = new Camera(glm::vec3(0.0f, 120.0f, -430.0f));
    projection = glm::perspective(glm::radians(FOV), (GLfloat)(SCREEN_W / SCREEN_H), NEAR_CLIP, FAR_CLIP);

    // Texture
    ImageLoader img;
    GLuint textureId = img.loadBMP_custom("./textures/circle.bmp");

    // Shaders
    Shader particleShader("particle.vert", "particle.frag");

    GLuint u_textureId = glGetUniformLocation(particleShader.id, "sampler");
    GLuint cameraRightId = glGetUniformLocation(particleShader.id, "cameraRight");
    GLuint cameraUpId = glGetUniformLocation(particleShader.id, "cameraUp");
    GLuint viewProjId = glGetUniformLocation(particleShader.id, "VP");




    ImageLoader img2;
    GLuint textureId2 = img2.loadBMP_custom("./textures/circle.bmp");

    Shader shaderBlur("blur.vert", "blur.frag");

    GLuint u_textureId2 = glGetUniformLocation(shaderBlur.id, "sampler");
    GLuint cameraRightId2 = glGetUniformLocation(shaderBlur.id, "cameraRight");
    GLuint cameraUpId2 = glGetUniformLocation(shaderBlur.id, "cameraUp");
    GLuint viewProjId2 = glGetUniformLocation(shaderBlur.id, "VP");





    sShader skyboxShader("skybox.vs", "skybox.fs");

    std::vector<std::string> faces
    {
        "./resource/skybox/right.jpg",
        "./resource/skybox/left.jpg",
        "./resource/skybox/top.jpg",
        "./resource/skybox/bottom.jpg",
        "./resource/skybox/front.jpg",
        "./resource/skybox/back.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    GLfloat* particle_position = new GLfloat[maxParticles * 4];
    GLubyte* particle_color = new GLubyte[maxParticles * 4];
    glm::vec3* pointlight_position = new glm::vec3[maxLights];
    glm::vec3* pointlight_color = new glm::vec3[maxLights];

    GLuint billboard_vertex_buffer;
    glGenBuffers(1, &billboard_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);

    GLuint particles_position_buffer;
    glGenBuffers(1, &particles_position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

    GLuint particles_color_buffer;
    glGenBuffers(1, &particles_color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(3);

    int nbFrames = 0;
    double lastTime = glfwGetTime();

    skyboxShader.SetUniform1i("skybox", 0);

    glClearColor(0, 0.1f, 0.2f, 0.8f);

    launcher.voise();

    while (!glfwWindowShouldClose(window))
    {
        nbFrames++;
        if (glfwGetTime() - lastTime >= 1.0) {
            //printf("%i fps\n", nbFrames);
            nbFrames = 0;
            lastTime += 1.0;
        }

        Camera::updateDeltaTime();
        launcher.update(*camera, particle_position, particle_color, pointlight_position, pointlight_color);
        /*
        for (int i = 0; i < maxLights; i++) {
            std::cout << i << " " << pointlight_position[i].x << " " << pointlight_position[i].y << " " << pointlight_position[i].z << std::endl;
            std::cout << i << " "  << pointlight_color[i].x << " " << pointlight_color[i].y << " " << pointlight_color[i].z  << std::endl;
        }
        */
        int launch_meteor = 0, launch_snow = 0;
        processInput(window, &launch_meteor, &launch_snow);
        if (launch_meteor == 1) {
            launcher.launchMeteor();
        }
        if (launch_snow == 1) {
            launcher.launchSnow();
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Projection
        auto viewMatrix = camera->getWorldToViewMatrix();
        auto vp = projection * viewMatrix;

        // Updating particle position buffer
        glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
        glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, Launcher::particlesCount * sizeof(GLfloat) * 4, particle_position);

        // Updating particle color buffer
        glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
        glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, Launcher::particlesCount * sizeof(GLubyte) * 4, particle_color);

        // Texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glUniform1i(u_textureId, 0);

        glEnable(GL_BLEND);


        particleShader.use();
        // Shader uniforms
        glUniform3f(cameraRightId, viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
        glUniform3f(cameraUpId, viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);
        glUniformMatrix4fv(viewProjId, 1, GL_FALSE, &vp[0][0]);

        // Quad vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Particle position
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Particle color
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
        glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);

        glVertexAttribDivisor(0, 0); // Quad vertices (4 for each particle)
        glVertexAttribDivisor(1, 1); // 1 position for each quad
        glVertexAttribDivisor(2, 1); // 1 color for each quad

        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, Launcher::particlesCount);

        shaderBlur.use();

        // Shader uniforms
        glUniform3f(cameraRightId, viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
        glUniform3f(cameraUpId, viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);
        glUniformMatrix4fv(viewProjId, 1, GL_FALSE, &vp[0][0]);

        // Quad vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Particle position
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Particle color
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
        glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);

        glVertexAttribDivisor(0, 0); // Quad vertices (4 for each particle)
        glVertexAttribDivisor(1, 1); // 1 position for each quad
        glVertexAttribDivisor(2, 1); // 1 color for each quad

        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, Launcher::particlesCount);



        skyboxShader.use();
        glDepthFunc(GL_LEQUAL);
        //viewMatrix = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skyboxShader.SetUniformMat4("view", viewMatrix);
        skyboxShader.SetUniformMat4("projection", projection);

        skyboxShader.SetUniform3f("viewPos", camera->getPosition());
        skyboxShader.SetUniform3f("normal", glm::vec3(0.0, 1.0, 0.0));

        skyboxShader.SetUniform3f("pointlights[0].position", pointlight_position[0]);
        skyboxShader.SetUniform3f("pointlights[0].color", pointlight_color[0]);
        skyboxShader.SetUniform3f("pointlights[1].position", pointlight_position[1]);
        skyboxShader.SetUniform3f("pointlights[1].color", pointlight_color[1]);
        skyboxShader.SetUniform3f("pointlights[2].position", pointlight_position[2]);
        skyboxShader.SetUniform3f("pointlights[2].color", pointlight_color[2]);
        skyboxShader.SetUniform3f("pointlights[3].position", pointlight_position[3]);
        skyboxShader.SetUniform3f("pointlights[3].color", pointlight_color[3]);
        skyboxShader.SetUniform3f("pointlights[4].position", pointlight_position[4]);
        skyboxShader.SetUniform3f("pointlights[4].color", pointlight_color[4]);
        skyboxShader.SetUniform3f("pointlights[5].position", pointlight_position[5]);
        skyboxShader.SetUniform3f("pointlights[5].color", pointlight_color[5]);
        skyboxShader.SetUniform3f("pointlights[6].position", pointlight_position[6]);
        skyboxShader.SetUniform3f("pointlights[6].color", pointlight_color[6]);
        skyboxShader.SetUniform3f("pointlights[7].position", pointlight_position[7]);
        skyboxShader.SetUniform3f("pointlights[7].color", pointlight_color[7]);
        skyboxShader.SetUniform3f("pointlights[8].position", pointlight_position[8]);
        skyboxShader.SetUniform3f("pointlights[8].color", pointlight_color[8]);
        skyboxShader.SetUniform3f("pointlights[9].position", pointlight_position[9]);
        skyboxShader.SetUniform3f("pointlights[9].color", pointlight_color[9]);
        skyboxShader.SetUniform3f("pointlights[10].position", pointlight_position[10]);
        skyboxShader.SetUniform3f("pointlights[10].color", pointlight_color[10]);
        skyboxShader.SetUniform3f("pointlights[11].position", pointlight_position[11]);
        skyboxShader.SetUniform3f("pointlights[11].color", pointlight_color[11]);
        skyboxShader.SetUniform3f("pointlights[12].position", pointlight_position[12]);
        skyboxShader.SetUniform3f("pointlights[12].color", pointlight_color[12]);
        skyboxShader.SetUniform3f("pointlights[13].position", pointlight_position[13]);
        skyboxShader.SetUniform3f("pointlights[13].color", pointlight_color[13]);
        skyboxShader.SetUniform3f("pointlights[14].position", pointlight_position[14]);
        skyboxShader.SetUniform3f("pointlights[14].color", pointlight_color[14]);
        skyboxShader.SetUniform3f("pointlights[15].position", pointlight_position[15]);
        skyboxShader.SetUniform3f("pointlights[15].color", pointlight_color[15]);

        skyboxShader.SetUniform3f("amb", glm::vec3(0.05f, 0.05f, 0.05f));
        skyboxShader.SetUniform3f("dif", glm::vec3(0.8f, 0.8f, 0.8f));
        skyboxShader.SetUniform3f("spe", glm::vec3(1.0f, 1.0f, 1.0f));
        skyboxShader.SetUniform1f("constant", 1.0f);
        skyboxShader.SetUniform1f("linear", 0.09f);
        skyboxShader.SetUniform1f("quadratic", 0.032f);
        skyboxShader.SetUniform1f("shininess", 32.0f);



        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(3);
        glDepthFunc(GL_LESS);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete[] particle_position;
    delete[] particle_color;

    glDeleteBuffers(1, &particles_color_buffer);
    glDeleteBuffers(1, &particles_position_buffer);
    glDeleteBuffers(1, &billboard_vertex_buffer);
    glDeleteTextures(1, &u_textureId);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glfwTerminate();
    return 0;
}

void updateCameraRotation(GLFWwindow* window)
{
    glfwGetCursorPos(window, &xPos, &yPos);

    yaw += (xPos - lastXPos) * SENSITIVITY;
    pitch += (lastYPos - yPos) * SENSITIVITY;
    pitch = clamp(pitch, 89.0f, -89.0f);

    lastXPos = xPos;
    lastYPos = yPos;

    camera->rotate((float)yaw, (float)pitch);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_REPEAT)
        return;
    heldKeys[key] = action == GLFW_PRESS;
}

void processInput(GLFWwindow* window, int* launch_meteor, int* launch_snow)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Camera controls
    updateCameraRotation(window);

    if (heldKeys[GLFW_KEY_SPACE])
        camera->moveUp(10.0f);
    if (heldKeys[GLFW_KEY_LEFT_SHIFT])
        camera->moveUp(-10.0f);
    if (heldKeys[GLFW_KEY_W])
        camera->moveForward(10.0f);
    if (heldKeys[GLFW_KEY_S])
        camera->moveForward(-10.0f);
    if (heldKeys[GLFW_KEY_D])
        camera->moveLeft(10.0f);
    if (heldKeys[GLFW_KEY_A])
        camera->moveLeft(-10.0f);

    if (heldKeys[GLFW_KEY_ENTER])
        *launch_meteor = 1;
    if (heldKeys[GLFW_KEY_RIGHT_SHIFT])
        *launch_snow = 1;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    SCREEN_W = (float)width;
    SCREEN_H = (float)height;
    projection = glm::perspective(glm::radians(FOV), (GLfloat)(SCREEN_W / SCREEN_H), NEAR_CLIP, FAR_CLIP);

    glViewport(0, 0, width, height);
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
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


GLuint* getAttachmentTexture(GLuint num)
{
    GLuint* textureID;
    textureID = new GLuint[num];
    glGenTextures(num, textureID);
    for (GLuint i = 0; i < num; i++)
    {
        glBindTexture(GL_TEXTURE_2D, textureID[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCREEN_W, SCREEN_H, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}
