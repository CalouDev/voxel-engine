#include <iostream>
#include <string>
#include <math.h>
#include <filesystem>
#include <vector>
#include <map>

#include "../include/globals.h"
#include "../include/texture.h"
#include "../include/voxel.h"
#include "../include/flat_voxel.h"
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

bool showDebugText = true;
bool canShowDebugText = true;

int inventory_case_selected = 0;

struct Character {
    unsigned int textureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    long int Advance;
};

std::map<char, Character> Characters;

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
        float next_y = (showDebugText ? camera_player.getDirection().y : 0.0f);
        camera_player.move((camera_spd * glm::vec3(camera_player.getDirection().x, next_y, camera_player.getDirection().z)));
    }
    
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        float next_y = (showDebugText ? camera_player.getDirection().y : 0.0f);
        camera_player.move(-(camera_spd * glm::vec3(camera_player.getDirection().x, next_y, camera_player.getDirection().z)));
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
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) {
        if (!canCrouch) {
            camera_player.move(glm::vec3(0.0f, 0.75f, 0.0f));
        }
        canCrouch = true;
    }

    if (glfwGetKey(window, GLFW_KEY_F9) == GLFW_PRESS) {
        if (canShowDebugText) {
            showDebugText = !showDebugText;
            canShowDebugText = false;
        }
    } else if (glfwGetKey(window, GLFW_KEY_F9) == GLFW_RELEASE) {
        canShowDebugText = true;
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
    if (yoffset > 0) {
        inventory_case_selected = (inventory_case_selected + 1) % 8;
    } else {
        inventory_case_selected = (inventory_case_selected - 1) % 8;
    }
}

void RenderText(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color, unsigned int VAO, unsigned int VBO) {
    s.setVec3("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    float original_x = x;

    for (char c : text) {
        if (c == '\n') {
            y -= Characters['H'].Size.y * scale * LINE_SPACING;
            x = original_x;
            continue;
        }

        Character ch = Characters[c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.Advance >> 6) * scale;
    }
}



int main(void) {
    srand(time(0));

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

    GLFWwindow* window = glfwCreateWindow(WINDOW::WIDTH, WINDOW::HEIGHT, "Voxel Engine - OpenGL 3.3",  NULL, NULL);
    
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

    VoxelManager main_voxel_manager;

    // Change to map<string, data>
    main_voxel_manager.addVoxel(Voxel(TEX::DIRT::SIDE, TEX::DIRT::TOP, TEX::DIRT::BOT, TEX::DIRT::WIDTH, TEX::DIRT::HEIGHT));
    main_voxel_manager.addVoxel(Voxel(TEX::WALL::SIDE, TEX::WALL::TOP, TEX::WALL::BOT, TEX::WALL::WIDTH, TEX::WALL::HEIGHT));
    main_voxel_manager.addVoxel(Voxel(TEX::GLASS::SIDE, TEX::GLASS::TOP, TEX::GLASS::BOT, TEX::GLASS::WIDTH, TEX::GLASS::HEIGHT));
    main_voxel_manager.addVoxel(Voxel(TEX::WOOD::SIDE, TEX::WOOD::TOP, TEX::WOOD::BOT, TEX::WOOD::WIDTH, TEX::WOOD::HEIGHT));
    main_voxel_manager.addVoxel(Voxel(TEX::LEAVES::SIDE, TEX::LEAVES::SIDE, TEX::LEAVES::SIDE, TEX::WOOD::WIDTH, TEX::WOOD::HEIGHT));
    main_voxel_manager.addVoxel(Voxel(TEX::DIRT_SNOW::SIDE, TEX::DIRT_SNOW::TOP, TEX::DIRT_SNOW::BOT, TEX::DIRT_SNOW::WIDTH, TEX::DIRT_SNOW::HEIGHT));
    main_voxel_manager.addFlatVoxel(FlatVoxel(TEX::GRASS::SIDE, TEX::GRASS::WIDTH, TEX::GRASS::HEIGHT));
    main_voxel_manager.addFlatVoxel(FlatVoxel(TEX::DAISY::SIDE, TEX::DAISY::WIDTH, TEX::DAISY::HEIGHT));
    main_voxel_manager.addFlatVoxel(FlatVoxel(TEX::ROSE::SIDE, TEX::ROSE::WIDTH, TEX::ROSE::HEIGHT));

    main_voxel_manager.use();

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

    Shader shader_flat_voxel("./shaders/shader_vs_flat_voxel.glsl", "./shaders/shader_fs_flat_voxel.glsl");
    shader_flat_voxel.use();
    
    Shader lightingShader("./shaders/shader_vs_light.glsl", "./shaders/shader_fs_light.glsl");
    lightingShader.use();
    lightingShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

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

    float inventory_vertices[] = {
        WINDOW::WIDTH / 2.0f - 128.0f, 10.0f, TEX::INVENTORY::SIDE.x, TEX::INVENTORY::SIDE.y,
        WINDOW::WIDTH / 2.0f + 128.0f, 10.0f, TEX::INVENTORY::SIDE.x + TEX::INVENTORY::WIDTH, TEX::INVENTORY::SIDE.y,
        WINDOW::WIDTH / 2.0f + 128.0f, 42.0f, TEX::INVENTORY::SIDE.x + TEX::INVENTORY::WIDTH, TEX::INVENTORY::SIDE.y + TEX::INVENTORY::HEIGHT,
        WINDOW::WIDTH / 2.0f + 128.0f, 42.0f, TEX::INVENTORY::SIDE.x + TEX::INVENTORY::WIDTH, TEX::INVENTORY::SIDE.y + TEX::INVENTORY::HEIGHT,
        WINDOW::WIDTH / 2.0f - 128.0f, 42.0f, TEX::INVENTORY::SIDE.x, TEX::INVENTORY::SIDE.y + TEX::INVENTORY::HEIGHT,
        WINDOW::WIDTH / 2.0f - 128.0f, 10.0f, TEX::INVENTORY::SIDE.x, TEX::INVENTORY::SIDE.y
    };

    unsigned int inventoryVBO;
    glGenBuffers(1, &inventoryVBO);
    glBindBuffer(GL_ARRAY_BUFFER, inventoryVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(inventory_vertices), inventory_vertices, GL_STATIC_DRAW);

    unsigned int inventoryVAO;
    glGenVertexArrays(1, &inventoryVAO);
    glBindVertexArray(inventoryVAO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Shader shaderGui("./shaders/shader_vs_gui.glsl", "./shaders/shader_fs_gui.glsl");
    shaderGui.use();

    Shader crosshairShader("./shaders/shader_vs_default_2d.glsl", "./shaders/shader_fs_default_2d.glsl");
    crosshairShader.use();

    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 model;

    std::vector<glm::vec2> grass_map;
    std::vector<glm::vec2> daisy_map;
    std::vector<glm::vec2> rose_map;

    for (int i = -19; i <= 19; ++i) {
        for (int j = -19; j <= 19; ++j) {
            int r = rand() % 30;
            if (r == 1) {
                grass_map.push_back(glm::vec2(i, j));
            } else if (r == 2) {
                daisy_map.push_back(glm::vec2(i, j));
            } else if (r == 3) {
                rose_map.push_back(glm::vec2(i, j));
            }
        }
    }

    Shader textShader("./shaders/shader_vs_glyph.glsl", "./shaders/shader_fs_glyph.glsl");
    textShader.use();

    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    FT_Face face;
    if (FT_New_Face(ft, "./fonts/vcr_osd_mono.ttf", 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    FT_Set_Pixel_Sizes(face, 0, FONT_SIZE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // no byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++) {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    unsigned int textVAO, textVBO;
    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6*4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

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

        shader_flat_voxel.use();
        shader_flat_voxel.setMat4("view", view);
        shader_flat_voxel.setMat4("projection", projection);
        shader_flat_voxel.setInt("Material", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(main_voxel_manager.getFlatVoxelManager()[0].getVAO());

        for (glm::vec2 plant_coord : grass_map) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(plant_coord.x, 1.0f, plant_coord.y));
            shader_flat_voxel.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(main_voxel_manager.getFlatVoxelManager()[1].getVAO());

        for (glm::vec2 plant_coord : daisy_map) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(plant_coord.x, 1.0f, plant_coord.y));
            shader_flat_voxel.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(main_voxel_manager.getFlatVoxelManager()[2].getVAO());

        for (glm::vec2 plant_coord : rose_map) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(plant_coord.x, 1.0f, plant_coord.y));
            shader_flat_voxel.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        lightingShader.use();
        lightingShader.setVec3("light.position", camera_player.getPos());
        lightingShader.setVec3("light.direction", camera_player.getDirection());
        lightingShader.setFloat("light.cutOff",  glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
        lightingShader.setVec3("light.ambient", glm::vec3(0.6f));
        lightingShader.setVec3("light.diffuse", glm::vec3(1.5f));
        lightingShader.setVec3("light.specular", glm::vec3(1.0f));
        lightingShader.setBool("isFlashlight", isFlashlight);
        //lightingShader.setBool("spotLight.on", isFlashlight);
        /*lightingShader.setVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        lightingShader.setVec3("spotLight.diffuse", glm::vec3(0.5f));
        lightingShader.setVec3("spotLight.specular", glm::vec3(0.1f));*/
        /*lightingShader.setVec3("pointLights[0].pos", lightPos);
        lightingShader.setVec3("pointLights[0].ambient", glm::vec3(0.4f, 0.4f, 0.4f));
        lightingShader.setVec3("pointLights[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setFloat("pointLights[0].constant", ATTENUATIONS[9].constant);
        lightingShader.setFloat("pointLights[0].linear", ATTENUATIONS[9].linear);
        lightingShader.setFloat("pointLights[0].quadratic", ATTENUATIONS[9].quadratic);*/
        /*lightingShader.setVec3("pointLights[1].pos", glm::vec3(2.0f, 3.0f, 5.0f));
        lightingShader.setVec3("pointLights[1].ambient", glm::vec3(0.4f, 0.4f, 0.4f));
        lightingShader.setVec3("pointLights[1].diffuse", glm::vec3(0.7f, 0.7f, 0.7f));
        lightingShader.setVec3("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setFloat("pointLights[1].constant", 1.0f);
        lightingShader.setFloat("pointLights[1].linear", 0.045f);
        lightingShader.setFloat("pointLights[1].quadratic", 0.075f);*/
        lightingShader.setFloat("light.constant", 1.0f);
        lightingShader.setFloat("light.linear", 0.022f);
        lightingShader.setFloat("light.quadratic", 0.0019f);
        lightingShader.setInt("material.diffuse", 0);
        lightingShader.setInt("material.specular", 0);
        lightingShader.setFloat("material.shininess", 32.0f);
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);
        lightingShader.setVec3("cameraPos", camera_player.getPos());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(main_voxel_manager.getVoxelManager()[0].getVAO());

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
        glBindVertexArray(main_voxel_manager.getVoxelManager()[1].getVAO());

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
                        if (j == 1 && k == 1 && i == 2) continue;
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

        // GLASS
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(main_voxel_manager.getVoxelManager()[2].getVAO());
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 2.0f, 5.0f));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // WOOD
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(main_voxel_manager.getVoxelManager()[3].getVAO());
        for (int i = 0; i < 6; ++i) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(11.0f, 1.0f + (float)i, -11.0f));
            lightingShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // LEAVES
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(main_voxel_manager.getVoxelManager()[4].getVAO()); // "LEAVES"
        for (int i = 0; i < 2; ++i) {
            for (int j = -2; j <= 2; ++j) {
                for (int k = -2; k <= 2; ++k) {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(11.0f + (float)j, 5.0f + (float)i, -11.0f + (float)k));
                    lightingShader.setMat4("model", model);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
            }
        }

        for (int i = 0; i < 2; ++i) {
            for (int j = -2 + i; j <= 2 - i; ++j) {
                for (int k = -2 + i; k <= 2 - i; ++k) {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(11.0f + (float)j, 7.0f + (float)i, -11.0f + (float)k));
                    lightingShader.setMat4("model", model);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
            }
        }

        // Snow
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(main_voxel_manager.getVoxelManager()[5].getVAO());

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-13.0f, 1.0f, -9.0f));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(VAO_door);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.0f, 1.5f, 4.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        crosshairShader.use();
        glBindVertexArray(crossHairVAO);
        projection = glm::ortho(0.0f, (float)WINDOW::WIDTH, 0.0f, (float)WINDOW::HEIGHT);
        crosshairShader.setMat4("projection", projection);
        glDrawArrays(GL_LINES, 0, 4);

        // Inventory
        shaderGui.use();
        shaderGui.setMat4("projection", projection);
        shaderGui.setInt("tex", 0);
        glBindVertexArray(inventoryVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        textShader.use();
        textShader.setMat4("projection", projection);
        std::ostringstream debug_text;
        // todo add color (green to ON, red to OFF)
        debug_text << "DEBUG MODE";
        if (showDebugText) {
            debug_text << std::fixed << std::setprecision(2) << " ON [ F9 ]";
            debug_text << "\nFPS : " << (int)(1.0f/deltaTime)/10 * 10;
            debug_text << "\nPosition : (" << camera_player.getPos().x << ", " << camera_player.getPos().y << ", " << camera_player.getPos().z << ")";
            debug_text << "\nDirection : (" << camera_player.getDirection().x << ", " << camera_player.getDirection().y << ", " << camera_player.getDirection().z << ")";
        } else {
            debug_text << " OFF [ F9 ]";
        }

        RenderText(textShader, debug_text.str(), 10.0f, (float)WINDOW::HEIGHT - FONT_SIZE - 1.0f, 1.0f, glm::vec3(1.0, 1.0f, 1.0f), textVAO, textVBO);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    main_voxel_manager.destroy();
    glDeleteVertexArrays(1, &VAO_door);

    glfwTerminate();

    exit(EXIT_SUCCESS);
}
