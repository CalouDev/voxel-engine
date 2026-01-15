#include <iostream>
#include <string>
#include <math.h>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../include/shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#include "../include/globals.h"
#include "../include/texture.h"
#include "../include/voxel.h"

float camera_spd_const = 5.0f;
glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, -3.0f);
glm::vec3 cameraDirection = glm::normalize(glm::vec3(-1.0f, 0.0f, 0.0f));
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

float deltaTime, currentFrame;
float lastFrame = 0.0f;

float lastMouseX = 400.0f;
float lastMouseY = 300.0f;

float pitch = 0.0f, yaw = -90.0f;

bool firstMouse = true;

bool isFullscreen = false;
bool pressed = false;

glm::vec3 lightPos(1.0f, 3.0f, -2.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    float xoffset = xpos - lastMouseX;
    float yoffset = lastMouseY - ypos;
    lastMouseX = xpos;
    lastMouseY = ypos;
    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    else if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraDirection = glm::normalize(direction);
    cameraRight = glm::normalize(glm::cross(cameraDirection, up));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraDirection));


}

void processInput(GLFWwindow *window) {
    float camera_spd = camera_spd_const * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        pressed = false;

        if (!pressed) {
            pressed = true;
            if (!isFullscreen) {
                GLFWmonitor* primary = glfwGetPrimaryMonitor();
                const GLFWvidmode* mode = glfwGetVideoMode(primary);
                glfwSetWindowMonitor(window, primary, 0, 0, mode->width, mode->height, mode->refreshRate);
                isFullscreen = true;
            } else {
                glfwSetWindowMonitor(window, nullptr, 100, 100, WINDOW::WIDTH, WINDOW::HEIGHT, GLFW_DONT_CARE);
                isFullscreen = false;
            }
        }
    } else {
        pressed = false;
    }
    
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += (camera_spd * cameraDirection);
    }
    
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        cameraPos -= (camera_spd * cameraDirection);
    }
    
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        cameraPos -= (camera_spd * cameraRight);
    } 
    
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        cameraPos += (camera_spd * cameraRight);
    }
    
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        
    }
}

int main(void) {
    glm::mat4 trans = glm::mat4(1.0f);

    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW lib" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(WINDOW::WIDTH, WINDOW::HEIGHT, "",  NULL, NULL);
    
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    int major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    std::string title = "Running using OpenGL " + std::to_string(major) + "." + std::to_string(minor);
    glfwSetWindowTitle(window, title.c_str());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    Voxel dirt_voxel(TEX::DIRT::SIDE, TEX::DIRT::TOP, TEX::DIRT::BOT, TEX::DIRT::WIDTH, TEX::DIRT::HEIGHT);
    Voxel wall_voxel(TEX::WALL::SIDE, TEX::WALL::TOP, TEX::WALL::BOT, TEX::WALL::WIDTH, TEX::WALL::HEIGHT);

    dirt_voxel.use();
    wall_voxel.use();

    float door_vertices[] = {
        -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, TEX::DOOR::SIDE.x, TEX::DOOR::SIDE.y,
        -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, TEX::DOOR::SIDE.x, TEX::DOOR::SIDE.y + TEX::DOOR::HEIGHT/2,
        0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, TEX::DOOR::SIDE.x + TEX::DOOR::WIDTH, TEX::DOOR::SIDE.y,
        -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, TEX::DOOR::SIDE.x, TEX::DOOR::SIDE.y + TEX::DOOR::HEIGHT/2,
        0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, TEX::DOOR::SIDE.x + TEX::DOOR::WIDTH, TEX::DOOR::SIDE.y,
        0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, TEX::DOOR::SIDE.x + TEX::DOOR::WIDTH, TEX::DOOR::SIDE.y + TEX::DOOR::HEIGHT/2,

        -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, TEX::DOOR::SIDE.x, TEX::DOOR::SIDE.y + TEX::DOOR::HEIGHT/2,
        -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, TEX::DOOR::SIDE.x, TEX::DOOR::SIDE.y + TEX::DOOR::HEIGHT,
        0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, TEX::DOOR::SIDE.x + TEX::DOOR::WIDTH, TEX::DOOR::SIDE.y + TEX::DOOR::HEIGHT/2,
        -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, TEX::DOOR::SIDE.x, TEX::DOOR::SIDE.y + TEX::DOOR::HEIGHT,
        0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, TEX::DOOR::SIDE.x + TEX::DOOR::WIDTH, TEX::DOOR::SIDE.y + TEX::DOOR::HEIGHT/2,
        0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, TEX::DOOR::SIDE.x + TEX::DOOR::WIDTH, TEX::DOOR::SIDE.y + TEX::DOOR::HEIGHT,
    };

    unsigned int VAO_door;
    glGenVertexArrays(1, &VAO_door);
    glBindVertexArray(VAO_door);

    unsigned int VBO_door;
    glGenBuffers(1, &VBO_door);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_door);
    glBufferData(GL_ARRAY_BUFFER, sizeof(door_vertices), door_vertices, GL_STATIC_DRAW);

    /*
    unsigned int EBO_door;
    glGenBuffers(1, &EBO_door);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_door);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    */

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    Shader classicShader("./shaders/shader_vs_default.glsl", "./shaders/shader_fs_default.glsl");
    classicShader.use();

    stbi_set_flip_vertically_on_load(true);

    int tileset_width, tileset_height, nrChannels;
    unsigned char *tileset_data = stbi_load("./assets/sprites/tileset.png", &tileset_width, &tileset_height, &nrChannels, 0);

    if (!tileset_data) {
        std::cerr << "Failed to load texture" << std::endl;
        exit(EXIT_FAILURE);
    }

    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tileset_width, tileset_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tileset_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tileset_data);

    float light_vertices[] = {
        // positions // normals // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(light_vertices), light_vertices, GL_STATIC_DRAW);
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightSourceVAO;
    glGenVertexArrays(1, &lightSourceVAO);
    glBindVertexArray(lightSourceVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    Shader lightingShader("./shaders/shader_vs_light.glsl", "./shaders/shader_fs_light.glsl");
    lightingShader.use();
    glUniform3fv(glGetUniformLocation(lightingShader.getId(), "objectColor"), 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
    glUniform3fv(glGetUniformLocation(lightingShader.getId(), "lightColor"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

    int tileset_width2, tileset_height2, nrChannels2;
    unsigned char *tileset_data2 = stbi_load("./assets/sprites/container.png", &tileset_width2, &tileset_height2, &nrChannels2, 0);

    if (!tileset_data2) {
        std::cerr << "Failed to load texture" << std::endl;
        exit(EXIT_FAILURE);
    }

    unsigned int texture2;
    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tileset_width2, tileset_height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, tileset_data2);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tileset_data2);

    Shader lightingSourceShader("./shaders/shader_vs_light.glsl", "./shaders/shader_fs_light_source.glsl");
    lightingSourceShader.use();

    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 model;

    while(!glfwWindowShouldClose(window)) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view = glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);
        projection = glm::perspective(glm::radians(45.0f), (float)WINDOW::WIDTH / WINDOW::HEIGHT, 0.1f, 200.0f);

        classicShader.use();
        glUniformMatrix4fv(glGetUniformLocation(classicShader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(classicShader.getId(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(classicShader.getId(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(dirt_voxel.getVAO());
        glBindTexture(GL_TEXTURE_2D, texture);

        for (int i = -10; i <= 10; ++i) {
            for (int j = -10; j <= 10; ++j) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3((float)i, 0.0f, (float)j));
                glUniformMatrix4fv(glGetUniformLocation(classicShader.getId(), "model"), 1, 0, glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, 36);

                if (i == -10 || i == 10) {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3((float)i, 1.0f, (float)j));
                    glUniformMatrix4fv(glGetUniformLocation(classicShader.getId(), "model"), 1, 0, glm::value_ptr(model));
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }

                if (j == -10 || j == 10) {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3((float)i, 1.0f, (float)j));
                    glUniformMatrix4fv(glGetUniformLocation(classicShader.getId(), "model"), 1, 0, glm::value_ptr(model));
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
            }
        }


        glBindVertexArray(wall_voxel.getVAO());

        for (int j = 1; j <= 5; ++j) {
            if (j == 3) continue;

            for (int i = 1; i <= 3; ++i) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3((float)j, (float)i, 4.0f));
                glUniformMatrix4fv(glGetUniformLocation(classicShader.getId(), "model"), 1, 0, glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, 36);
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3((float)j, (float)i, 7.0f));
                glUniformMatrix4fv(glGetUniformLocation(classicShader.getId(), "model"), 1, 0, glm::value_ptr(model));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            if (j == 1 || j == 5) {
                for (int k = 1; k <= 2; ++k) {
                    for (int i = 1; i <= 3; ++i) {
                        model = glm::mat4(1.0f);
                        model = glm::translate(model, glm::vec3((float)j, (float)i, 4.0f + (float)k));
                        glUniformMatrix4fv(glGetUniformLocation(classicShader.getId(), "model"), 1, 0, glm::value_ptr(model));
                        glDrawArrays(GL_TRIANGLES, 0, 36);
                    }
                }
            }
        }

        for (int i = 1; i <= 3; ++i) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(3.0f, (float)i, 7.0f));
            glUniformMatrix4fv(glGetUniformLocation(classicShader.getId(), "model"), 1, 0, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.0f, 3.0f, 4.0f));
        glUniformMatrix4fv(glGetUniformLocation(classicShader.getId(), "model"), 1, 0, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(VAO_door);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.0f, 1.5f, 4.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(classicShader.getId(), "model"), 1, 0, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 12);

        lightingShader.use();
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform3fv(glGetUniformLocation(lightingShader.getId(), "objectColor"), 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
        glUniform3fv(glGetUniformLocation(lightingShader.getId(), "lightColor"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        glUniform3fv(glGetUniformLocation(lightingShader.getId(), "lightPos"), 1, glm::value_ptr(lightPos));
        lightingShader.setVec3("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
        lightingShader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        lightingShader.setVec3("material.specular", glm::vec3(1.0f));
        lightingShader.setFloat("material.shininess", 32.0f);
        lightingShader.setVec3("light.ambient", glm::vec3(0.4f, 0.4f, 0.4f));
        lightingShader.setVec3("light.diffuse", glm::vec3(0.7f, 0.7f, 0.7f));
        lightingShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        glBindVertexArray(lightVAO);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.getId(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.getId(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3fv(glGetUniformLocation(lightingShader.getId(), "playerPos"), 1, glm::value_ptr(cameraPos));
        glBindTexture(GL_TEXTURE_2D, texture2);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(4.0f, 1.0f, 3.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lightingSourceShader.use();
        glBindVertexArray(lightVAO);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        glUniformMatrix4fv(glGetUniformLocation(lightingSourceShader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(lightingSourceShader.getId(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(lightingSourceShader.getId(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    dirt_voxel.destroy();
    wall_voxel.destroy();

    glfwTerminate();

    exit(EXIT_SUCCESS);
}
