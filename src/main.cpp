#include <iostream>
#include <string>
#include <math.h>
#include <filesystem>

#include "../include/globals.h"
#include "../include/texture.h"
#include "../include/voxel.h"
#include "../include/voxel_manager.h"
#include "../include/shader.h"
#include "../include/camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

Camera camera_player(PLAYER::DEFAULT_POS, glm::normalize(glm::vec3(0.0f)));

float deltaTime, currentFrame;
float lastFrame = 0.0f;

float lastMouseX = WINDOW::WIDTH/2.0f;
float lastMouseY = WINDOW::HEIGHT/2.0f;

float pitch = 0.0f;
float yaw = -90.0f;
float sensitivity = 0.05f;

bool isFullscreen = false;
bool pressed = false;

glm::vec3 lightPos(3.0f, 10.0f, 0.0f);

float fov = 45.0f;

bool isFlashlight = false;
bool canFlashlight = true;

bool canCrouch = true;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    float xoffset = xpos - lastMouseX;
    float yoffset = lastMouseY - ypos;
    lastMouseX = xpos;
    lastMouseY = ypos;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    } else if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    glm::vec3 new_direction;
    new_direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    new_direction.y = sin(glm::radians(pitch));
    new_direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    camera_player.setDirection(new_direction);
    camera_player.setRight(glm::normalize(glm::cross(camera_player.getDirection(), SPACE::VECTOR_UP)));
    camera_player.setUp(glm::normalize(glm::cross(camera_player.getRight(), camera_player.getDirection())));
}

void processInput(GLFWwindow *window) {
    float camera_spd = 8.0f * deltaTime;
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
        camera_player.move((camera_spd * glm::vec3(camera_player.getDirection().x, 0.0f, camera_player.getDirection().z)));
    }
    
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera_player.move(-(camera_spd * glm::vec3(camera_player.getDirection().x, 0.0f, camera_player.getDirection().z)));
    }
    
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera_player.move(-(camera_spd * camera_player.getRight()));
    } 
    
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera_player.move(camera_spd * camera_player.getRight());
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        if (canCrouch) {
            camera_player.move(glm::vec3(0.0f, -0.75f, 0.0f));
            canCrouch = false;
        }
    } else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE) {
        if (!canCrouch) {
            camera_player.move(glm::vec3(0.0f, 0.75f, 0.0f));
        }
        canCrouch = true;
    }
    
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        if (canFlashlight) {
            isFlashlight = !isFlashlight;
            canFlashlight = false;
        }
    } else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE) {
        canFlashlight = true;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= (float)yoffset;
    if (fov < 1.0f) {
        fov = 1.0f;
    } else if (fov > 45.0f) {
        fov = 45.0f;
    }
}

int main(void) {
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW lib" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(WINDOW::WIDTH, WINDOW::HEIGHT, "OpenGL 3.3",  NULL, NULL);
    
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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    VoxelManager voxel_manager;

    voxel_manager.add(Voxel(TEX::DIRT::SIDE, TEX::DIRT::TOP, TEX::DIRT::BOT, TEX::DIRT::WIDTH, TEX::DIRT::HEIGHT));
    voxel_manager.add(Voxel(TEX::WALL::SIDE, TEX::WALL::TOP, TEX::WALL::BOT, TEX::WALL::WIDTH, TEX::WALL::HEIGHT));

    voxel_manager.use();

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
    
    //Shader classicShader("./shaders/shader_vs_default.glsl", "./shaders/shader_fs_default.glsl");
    //classicShader.use();

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
    //glUniform3fv(glGetUniformLocation(lightingShader.getId(), "objectColor"), 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
    lightingShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    int tileset_width2, tileset_height2, nrChannels2;
    unsigned char *tileset_data2 = stbi_load("./assets/sprites/container.png", &tileset_width2, &tileset_height2, &nrChannels2, 0);

    if (!tileset_data2) {
        std::cerr << "Failed to load texture" << std::endl;
        exit(EXIT_FAILURE);
    }

    unsigned int texture2;
    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tileset_width2, tileset_height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, tileset_data2);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tileset_data2);

    int container_specular_map_width, container_specular_map_height, container_specular_map_nr_channels;
    unsigned char *container_specular_map_data = stbi_load("./assets/sprites/container2_specular.png", &container_specular_map_width, &container_specular_map_height, &container_specular_map_nr_channels, 0);

    if (!container_specular_map_data) {
        std::cerr << "Failed to load texture" << std::endl;
        exit(EXIT_FAILURE);
    }

    unsigned int tex_container_specular_map;
    glGenTextures(1, &tex_container_specular_map);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, tex_container_specular_map);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, container_specular_map_width, container_specular_map_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, container_specular_map_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(container_specular_map_data);

    Shader lightingSourceShader("./shaders/shader_vs_light.glsl", "./shaders/shader_fs_light_source.glsl");
    lightingSourceShader.use();

    float crosshair_vertices[] = {
        WINDOW::WIDTH/2 - 10, WINDOW::HEIGHT/2, 1.0f, 1.0f, 1.0f, 1.0f,
        WINDOW::WIDTH/2 + 10, WINDOW::HEIGHT/2, 1.0f, 1.0f, 1.0f, 1.0f,
        WINDOW::WIDTH/2, WINDOW::HEIGHT/2 - 10, 1.0f, 1.0f, 1.0f, 1.0f,
        WINDOW::WIDTH/2, WINDOW::HEIGHT/2 + 10, 1.0f, 1.0f, 1.0f, 1.0f
    };

    unsigned int crosshairVBO;
    glGenBuffers(1, &crosshairVBO);
    glBindBuffer(GL_ARRAY_BUFFER, crosshairVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crosshair_vertices), crosshair_vertices, GL_STATIC_DRAW);

    unsigned int crossHairVAO;
    glGenVertexArrays(1, &crossHairVAO);
    glBindVertexArray(crossHairVAO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Shader crosshairShader("./shaders/shader_vs_default_2d.glsl", "./shaders/shader_fs_default_2d.glsl");
    crosshairShader.use();

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

        view = glm::lookAt(camera_player.getPos(), camera_player.getPos() + camera_player.getDirection(), camera_player.getUp());
        projection = glm::perspective(glm::radians(fov), (float)WINDOW::WIDTH / WINDOW::HEIGHT, 0.1f, 200.0f);

        lightingShader.use();
        lightingShader.setVec3("dirLight.direction", glm::vec3(0.0f, -0.1f, 0.0f));
        lightingShader.setVec3("dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        lightingShader.setVec3("dirLight.diffuse", glm::vec3(0.1f));
        lightingShader.setVec3("dirLight.specular", glm::vec3(0.1f));
        lightingShader.setVec3("spotLight.position", camera_player.getPos());
        lightingShader.setVec3("spotLight.direction", camera_player.getDirection());
        lightingShader.setBool("spotLight.on", isFlashlight);
        lightingShader.setVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        lightingShader.setVec3("spotLight.diffuse", glm::vec3(0.5f));
        lightingShader.setVec3("spotLight.specular", glm::vec3(0.1f));
        lightingShader.setFloat("spotlight.cutOff",  1.25f);
        lightingShader.setFloat("spotlight.outerCutOff", 1.75);
        lightingShader.setVec3("pointLights[0].pos", lightPos);
        lightingShader.setVec3("pointLights[0].ambient", glm::vec3(0.4f, 0.4f, 0.4f));
        lightingShader.setVec3("pointLights[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setFloat("pointLights[0].constant", ATTENUATIONS[9].constant);
        lightingShader.setFloat("pointLights[0].linear", ATTENUATIONS[9].linear);
        lightingShader.setFloat("pointLights[0].quadratic", ATTENUATIONS[9].quadratic);
        /*lightingShader.setVec3("pointLights[1].pos", glm::vec3(2.0f, 3.0f, 5.0f));
        lightingShader.setVec3("pointLights[1].ambient", glm::vec3(0.4f, 0.4f, 0.4f));
        lightingShader.setVec3("pointLights[1].diffuse", glm::vec3(0.7f, 0.7f, 0.7f));
        lightingShader.setVec3("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setFloat("pointLights[1].constant", 1.0f);
        lightingShader.setFloat("pointLights[1].linear", 0.045f);
        lightingShader.setFloat("pointLights[1].quadratic", 0.075f);*/
        lightingShader.setInt("material.diffuse", 0);
        lightingShader.setInt("material.specular", 0);
        lightingShader.setFloat("material.shininess", 0.0f);
        lightingShader.setBool("noSpecular", true); // change with a black/with texture (specular map full or none)
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);
        lightingShader.setVec3("cameraPos", camera_player.getPos());
        //lightingShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(voxel_manager.getManager()[0].getVAO());

        for (int i = -20; i <= 20; ++i) {
            for (int j = -20; j <= 20; ++j) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3((float)i, 0.0f, (float)j));
                lightingShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);

                if (i == -20 || i == 20) {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3((float)i, 1.0f, (float)j));
                    lightingShader.setMat4("model", model);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }

                if (j == -20 || j == 20) {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3((float)i, 1.0f, (float)j));
                    lightingShader.setMat4("model", model);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
            }
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(voxel_manager.getManager()[1].getVAO());

        for (int j = 1; j <= 5; ++j) {
            if (j == 3) continue;

            for (int i = 1; i <= 3; ++i) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3((float)j, (float)i, 4.0f));
                lightingShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3((float)j, (float)i, 7.0f));
                lightingShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            if (j == 1 || j == 5) {
                for (int k = 1; k <= 2; ++k) {
                    for (int i = 1; i <= 3; ++i) {
                        model = glm::mat4(1.0f);
                        model = glm::translate(model, glm::vec3((float)j, (float)i, 4.0f + (float)k));
                        lightingShader.setMat4("model", model);
                        glDrawArrays(GL_TRIANGLES, 0, 36);
                    }
                }
            }
        }

        for (int i = 1; i <= 3; ++i) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(3.0f, (float)i, 7.0f));
            lightingShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.0f, 3.0f, 4.0f));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(VAO_door);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.0f, 1.5f, 4.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        glBindVertexArray(lightVAO);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, tex_container_specular_map);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
        lightingShader.setMat4("model", model);
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);
        lightingShader.setVec3("cameraPos", camera_player.getPos());
        lightingShader.setInt("material.diffuse", 1);
        lightingShader.setInt("material.specular", 2);
        lightingShader.setFloat("material.shininess", 32.0f);
        lightingShader.setBool("noSpecular", false);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(4.0f, 1.0f, 3.0f));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lightingSourceShader.use();
        glBindVertexArray(lightVAO);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        lightingSourceShader.setMat4("model", model);
        lightingSourceShader.setMat4("view", view);
        lightingSourceShader.setMat4("projection", projection);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        crosshairShader.use();
        glBindVertexArray(crossHairVAO);
        projection = glm::ortho(0.0f, (float)WINDOW::WIDTH, 0.0f, (float)WINDOW::HEIGHT);
        crosshairShader.setMat4("projection", projection);
        glDrawArrays(GL_LINES, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    voxel_manager.destroy();
    glDeleteVertexArrays(1, &VAO_door);
    glDeleteVertexArrays(1, &lightVAO);

    glfwTerminate();

    exit(EXIT_SUCCESS);
}
