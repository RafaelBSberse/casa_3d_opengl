/*
 * Trabalho T2 - Casa 3D em OpenGL
 * Disciplina: Computação Gráfica
 * 
 * Este projeto implementa uma cena 3D de uma casa com múltiplos quartos,
 * objetos, texturas e iluminação Phong. O usuário pode navegar pela cena
 * usando uma câmera em primeira pessoa.
 * 
 * Bibliotecas: GLFW, GLEW, GLM, stb_image
 * Perfil: OpenGL 4.0 Core Profile
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "Model.h"
#include "Geometry.h"

// ========== CONFIGURAÇÕES DA JANELA ==========
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// ========== VARIÁVEIS GLOBAIS ==========
Camera camera(glm::vec3(0.0f, 1.7f, 8.0f)); // Câmera na altura dos olhos
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Iluminação
glm::vec3 lightPos(0.0f, 10.0f, 0.0f); // Luz bem acima, mais forte

// ========== CALLBACKS DO GLFW ==========

// Callback de redimensionamento de janela
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Callback de movimento do mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Invertido: y vai de baixo para cima

    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

// Callback de scroll do mouse
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.processMouseScroll(yoffset);
}

// Processa entrada do teclado
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Movimentação da câmera (WASD + Space/Shift)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(0, deltaTime); // Frente
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(1, deltaTime); // Trás
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(2, deltaTime); // Esquerda
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(3, deltaTime); // Direita
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.processKeyboard(4, deltaTime); // Cima
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.processKeyboard(5, deltaTime); // Baixo
}

// ========== FUNÇÃO PRINCIPAL ==========
int main() {
    // ========== INICIALIZAÇÃO DO GLFW ==========
    if (!glfwInit()) {
        std::cerr << "Falha ao inicializar GLFW" << std::endl;
        return -1;
    }

    // Configura versão do OpenGL (4.0 Core Profile)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Cria a janela
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Casa 3D - Trabalho T2", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Falha ao criar janela GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Registra callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Captura o mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // ========== INICIALIZAÇÃO DO GLEW ==========
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Falha ao inicializar GLEW" << std::endl;
        return -1;
    }

    // Configura viewport
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Ativa teste de profundidade
    glEnable(GL_DEPTH_TEST);

    // ========== CARREGA SHADERS ==========
    Shader shader("shaders/phong.vert", "shaders/phong.frag");

    // ========== CARREGA TEXTURAS ==========
    Texture* woodTexture = nullptr;
    Texture* wallTexture = nullptr;
    Texture* fabricTexture = nullptr;
    Texture* tileTexture = nullptr;
    Texture* metalTexture = nullptr;

    try {
        woodTexture = new Texture("res/textures/wood.jpg");
        std::cout << "Textura wood.jpg carregada com sucesso!" << std::endl;
    } catch(...) {
        std::cout << "Falha ao carregar wood.jpg" << std::endl;
    }

    try {
        wallTexture = new Texture("res/textures/wall.jpg");
        std::cout << "Textura wall.jpg carregada com sucesso!" << std::endl;
    } catch(...) {
        std::cout << "Falha ao carregar wall.jpg" << std::endl;
    }

    try {
        fabricTexture = new Texture("res/textures/fabric.jpg");
        std::cout << "Textura fabric.jpg carregada com sucesso!" << std::endl;
    } catch(...) {
        std::cout << "Falha ao carregar fabric.jpg" << std::endl;
    }

    try {
        tileTexture = new Texture("res/textures/tile.jpg");
        std::cout << "Textura tile.jpg carregada com sucesso!" << std::endl;
    } catch(...) {
        std::cout << "Falha ao carregar tile.jpg" << std::endl;
    }

    try {
        metalTexture = new Texture("res/textures/metal.jpg");
        std::cout << "Textura metal.jpg carregada com sucesso!" << std::endl;
    } catch(...) {
        std::cout << "Falha ao carregar metal.jpg" << std::endl;
    }

    // ========== CRIAÇÃO DA GEOMETRIA ==========
    std::vector<Model*> sceneObjects;

    // Chão
    Model* floor = new Model(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(20.0f, 0.1f, 20.0f));
    floor->addMesh(Geometry::createCube(woodTexture));
    sceneObjects.push_back(floor);

    // Teto
    Model* ceiling = new Model(glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(0.0f), glm::vec3(20.0f, 0.1f, 20.0f));
    ceiling->addMesh(Geometry::createCube(wallTexture));
    sceneObjects.push_back(ceiling);

    // Parede Norte
    Model* wallNorth = new Model(glm::vec3(0.0f, 2.0f, -10.0f), glm::vec3(0.0f), glm::vec3(20.0f, 4.0f, 0.2f));
    wallNorth->addMesh(Geometry::createCube(wallTexture));
    sceneObjects.push_back(wallNorth);

    // Parede Oeste
    Model* wallWest = new Model(glm::vec3(-10.0f, 2.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.2f, 4.0f, 20.0f));
    wallWest->addMesh(Geometry::createCube(wallTexture));
    sceneObjects.push_back(wallWest);

    // Parede Leste
    Model* wallEast = new Model(glm::vec3(10.0f, 2.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.2f, 4.0f, 20.0f));
    wallEast->addMesh(Geometry::createCube(wallTexture));
    sceneObjects.push_back(wallEast);

    // Parede Sul esquerda
    Model* wallSouth_Left = new Model(glm::vec3(-5.5f, 2.0f, 10.0f), glm::vec3(0.0f), glm::vec3(9.0f, 4.0f, 0.2f));
    wallSouth_Left->addMesh(Geometry::createCube(wallTexture));
    sceneObjects.push_back(wallSouth_Left);

    // Parede Sul direita
    Model* wallSouth_Right = new Model(glm::vec3(5.5f, 2.0f, 10.0f), glm::vec3(0.0f), glm::vec3(9.0f, 4.0f, 0.2f));
    wallSouth_Right->addMesh(Geometry::createCube(wallTexture));
    sceneObjects.push_back(wallSouth_Right);

    // Divisória horizontal esquerda
    Model* divH_Left = new Model(glm::vec3(-6.0f, 2.0f, -3.0f), glm::vec3(0.0f), glm::vec3(8.0f, 4.0f, 0.2f));
    divH_Left->addMesh(Geometry::createCube(wallTexture));
    sceneObjects.push_back(divH_Left);

    // Pilar central
    Model* divH_Center = new Model(glm::vec3(0.0f, 3.0f, -3.0f), glm::vec3(0.0f), glm::vec3(0.3f, 2.0f, 0.2f));
    divH_Center->addMesh(Geometry::createCube(wallTexture));
    sceneObjects.push_back(divH_Center);

    // Divisória horizontal direita
    Model* divH_Right = new Model(glm::vec3(6.0f, 2.0f, -3.0f), glm::vec3(0.0f), glm::vec3(8.0f, 4.0f, 0.2f));
    divH_Right->addMesh(Geometry::createCube(wallTexture));
    sceneObjects.push_back(divH_Right);

    // Divisória vertical
    Model* divVertical = new Model(glm::vec3(0.0f, 2.0f, -6.5f), glm::vec3(0.0f), glm::vec3(0.2f, 4.0f, 7.0f));
    divVertical->addMesh(Geometry::createCube(wallTexture));
    sceneObjects.push_back(divVertical);

    // Acima da porta
    Model* doorTop = new Model(glm::vec3(0.0f, 3.0f, -3.0f), glm::vec3(0.0f), glm::vec3(4.0f, 2.0f, 0.2f));
    doorTop->addMesh(Geometry::createCube(wallTexture));
    sceneObjects.push_back(doorTop);

    // Porta banheiro
    Model* doorBathroom = new Model(glm::vec3(-2.0f, 1.0f, -3.0f), glm::vec3(0.0f), glm::vec3(2.0f, 2.0f, 0.08f));
    doorBathroom->addMesh(Geometry::createCube(woodTexture));
    sceneObjects.push_back(doorBathroom);

    // Porta QUARTO
    Model* doorBedroom = new Model(glm::vec3(2.0f, 1.0f, -3.0f), glm::vec3(0.0f), glm::vec3(2.0f, 2.0f, 0.08f));
    doorBedroom->addMesh(Geometry::createCube(woodTexture));
    sceneObjects.push_back(doorBedroom);

    // ==========================================
    // SALA DE ESTAR
    // ==========================================

    // Sofá
    Model* sofaBack = new Model(glm::vec3(-9.4f, 0.45f, 2.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(2.5f, 0.7f, 1.0f));
    sofaBack->addMesh(Geometry::createCube(fabricTexture));
    sceneObjects.push_back(sofaBack);

    // Encosto do sofá
    Model* sofaSeat = new Model(glm::vec3(-9.9f, 0.8f, 2.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(2.5f, 0.5f, 0.15f));
    sofaSeat->addMesh(Geometry::createCube(fabricTexture));
    sceneObjects.push_back(sofaSeat);

    // Mesa de centro
    Model* coffeeTableTop = new Model(glm::vec3(-5.5f, 0.45f, 2.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(1.5f, 0.06f, 0.9f));
    coffeeTableTop->addMesh(Geometry::createCube(woodTexture));
    sceneObjects.push_back(coffeeTableTop);

    // 4 pernas da mesa
    float legOffsetX = 0.38f;
    float legOffsetZ = 0.65f;
    for (int i = 0; i < 4; i++) {
        float x = -5.5f + ((i % 2) * 2 - 1) * legOffsetX;
        float z = 2.0f + ((i / 2) * 2 - 1) * legOffsetZ;
        Model* leg = new Model(glm::vec3(x, 0.225f, z), glm::vec3(0.0f), glm::vec3(0.06f, 0.45f, 0.06f));
        leg->addMesh(Geometry::createCube(woodTexture));
        sceneObjects.push_back(leg);
    }

    // === ÁREA DA TV ===
    
    // Rack da TV
    Model* tvRack = new Model(glm::vec3(9.6f, 0.35f, 2.0f), glm::vec3(0.0f), glm::vec3(0.8f, 0.7f, 3.0f));
    tvRack->addMesh(Geometry::createCube(woodTexture));
    sceneObjects.push_back(tvRack);

    // TV
    Model* tv = new Model(glm::vec3(9.9f, 1.5f, 2.0f), glm::vec3(0.0f, -90.0f, 0.0f), glm::vec3(2.2f, 1.3f, 0.08f));
    tv->addMesh(Geometry::createCube(nullptr));
    sceneObjects.push_back(tv);

    // === ILUMINAÇÃO ===
    
    // Luminária de chão
    Model* lampBase = new Model(glm::vec3(9.6f, 0.15f, 0.3f), glm::vec3(0.0f), glm::vec3(0.3f, 0.3f, 0.3f));
    lampBase->addMesh(Geometry::createCube(woodTexture));
    sceneObjects.push_back(lampBase);

    Model* lampPole = new Model(glm::vec3(9.6f, 1.1f, 0.3f), glm::vec3(0.0f), glm::vec3(0.07f, 1.8f, 0.07f));
    lampPole->addMesh(Geometry::createCylinder(12, metalTexture));
    sceneObjects.push_back(lampPole);

    Model* lampShade = new Model(glm::vec3(9.6f, 2.05f, 0.3f), glm::vec3(0.0f), glm::vec3(0.4f));
    lampShade->addMesh(Geometry::createSphere(12, 12, nullptr));
    sceneObjects.push_back(lampShade);


    // ==========================================
    // QUARTO
    // ==========================================

    // === CAMA ===
    
    // Base da cama
    Model* bedFrame = new Model(glm::vec3(5.0f, 0.25f, -9.0f), glm::vec3(0.0f), glm::vec3(2.4f, 0.4f, 2.0f));
    bedFrame->addMesh(Geometry::createCube(woodTexture));
    sceneObjects.push_back(bedFrame);

    // Colchão
    Model* mattress = new Model(glm::vec3(5.0f, 0.5f, -9.0f), glm::vec3(0.0f), glm::vec3(2.2f, 0.3f, 1.8f));
    mattress->addMesh(Geometry::createCube(fabricTexture));
    sceneObjects.push_back(mattress);

    // Cabeceira
    Model* headboard = new Model(glm::vec3(5.0f, 0.9f, -9.85f), glm::vec3(0.0f), glm::vec3(2.2f, 1.0f, 0.1f));
    headboard->addMesh(Geometry::createCube(woodTexture));
    sceneObjects.push_back(headboard);

    // Travesseiro
    Model* pillow = new Model(glm::vec3(5.0f, 0.5f, -9.5f), glm::vec3(0.0f), glm::vec3(0.6f, 0.15f, 0.4f));
    pillow->addMesh(Geometry::createCube(fabricTexture));
    sceneObjects.push_back(pillow);

    // === MESA DE CABECEIRA ===
    
    Model* nightstand = new Model(glm::vec3(7.2f, 0.4f, -9.7f), glm::vec3(0.0f), glm::vec3(0.5f, 0.8f, 0.5f));
    nightstand->addMesh(Geometry::createCube(woodTexture));
    sceneObjects.push_back(nightstand);

    // Abajur na mesa
    Model* tableLampBase = new Model(glm::vec3(7.2f, 0.85f, -9.7f), glm::vec3(0.0f), glm::vec3(0.15f, 0.1f, 0.15f));
    tableLampBase->addMesh(Geometry::createCube(woodTexture));
    sceneObjects.push_back(tableLampBase);

    Model* tableLampPole = new Model(glm::vec3(7.2f, 1.05f, -9.7f), glm::vec3(0.0f), glm::vec3(0.04f, 0.3f, 0.04f));
    tableLampPole->addMesh(Geometry::createCylinder(8, metalTexture));
    sceneObjects.push_back(tableLampPole);

    Model* tableLampShade = new Model(glm::vec3(7.2f, 1.25f, -9.7f), glm::vec3(0.0f), glm::vec3(0.2f, 0.15f, 0.2f));
    tableLampShade->addMesh(Geometry::createCylinder(8, fabricTexture));
    sceneObjects.push_back(tableLampShade);

    // === GUARDA-ROUPA ===
    
    Model* wardrobe = new Model(glm::vec3(9.3f, 1.6f, -5.5f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(1.6f, 3.2f, 1.0f));
    wardrobe->addMesh(Geometry::createCube(woodTexture));
    sceneObjects.push_back(wardrobe);

    // === TAPETE DO QUARTO ===
    
    Model* bedroomRug = new Model(glm::vec3(5.0f, 0.005f, -6.5f), glm::vec3(0.0f), glm::vec3(3.0f, 0.01f, 2.5f));
    bedroomRug->addMesh(Geometry::createCube(fabricTexture));
    sceneObjects.push_back(bedroomRug);

    // ==========================================
    // BANHEIRO
    // ==========================================

    // === PISO DO BANHEIRO ===
    
    Model* bathroomFloor = new Model(glm::vec3(-5.0f, 0.01f, -6.5f), glm::vec3(0.0f), glm::vec3(10.0f, 0.02f, 7.0f));
    bathroomFloor->addMesh(Geometry::createCube(tileTexture));
    sceneObjects.push_back(bathroomFloor);

    // ==========================================
    // VASO SANITÁRIO
    // ==========================================
    
    // Base/pedestal
    Model* toiletBase = new Model(glm::vec3(-7.5f, 0.25f, -9.6f), glm::vec3(0.0f), glm::vec3(0.45f, 0.5f, 0.6f));
    toiletBase->addMesh(Geometry::createCube(tileTexture));
    sceneObjects.push_back(toiletBase);

    // Bacia principal
    Model* toiletBowl = new Model(glm::vec3(-7.5f, 0.55f, -9.6f), glm::vec3(0.0f), glm::vec3(0.40f, 0.25f, 0.40f));
    toiletBowl->addMesh(Geometry::createCylinder(20, tileTexture));
    sceneObjects.push_back(toiletBowl);

    // Aro do assento
    Model* toiletSeat = new Model(glm::vec3(-7.5f, 0.72f, -9.6f), glm::vec3(0.0f), glm::vec3(0.43f, 0.03f, 0.43f));
    toiletSeat->addMesh(Geometry::createCylinder(20, tileTexture));
    sceneObjects.push_back(toiletSeat);

    // Caixa acoplada
    Model* toiletTank = new Model(glm::vec3(-7.5f, 0.75f, -9.83f), glm::vec3(0.0f), glm::vec3(0.40f, 0.70f, 0.14f));
    toiletTank->addMesh(Geometry::createCube(tileTexture));
    sceneObjects.push_back(toiletTank);

    // Botão de descarga
    Model* flushButton = new Model(glm::vec3(-7.5f, 1.11f, -9.83f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.10f, 0.06f, 0.02f));
    flushButton->addMesh(Geometry::createCube(metalTexture));
    sceneObjects.push_back(flushButton);

    // ==========================================
    // PIA COM GABINETE
    // ==========================================
    
    // Gabinete
    Model* sinkCabinet = new Model(glm::vec3(-9.7f, 0.4f, -6.0f), glm::vec3(0.0f), glm::vec3(0.4f, 0.8f, 0.9f));
    sinkCabinet->addMesh(Geometry::createCube(woodTexture));
    sceneObjects.push_back(sinkCabinet);

    // Bancada/tampo
    Model* sinkCountertop = new Model(glm::vec3(-9.7f, 0.82f, -6.0f), glm::vec3(0.0f), glm::vec3(0.5f, 0.04f, 1.0f));
    sinkCountertop->addMesh(Geometry::createCube(tileTexture));
    sceneObjects.push_back(sinkCountertop);

    // Cuba da pia
    Model* sinkBasin = new Model(glm::vec3(-9.7f, 0.92f, -6.0f), glm::vec3(0.0f), glm::vec3(0.36f, 0.15f, 0.36f));
    sinkBasin->addMesh(Geometry::createCylinder(20, tileTexture));
    sceneObjects.push_back(sinkBasin);

    // === TORNEIRA ===
    
    // Base da torneira
    Model* faucetBase = new Model(glm::vec3(-9.8f, 1.0f, -6.0f), glm::vec3(0.0f), glm::vec3(0.08f, 0.04f, 0.08f));
    faucetBase->addMesh(Geometry::createCube(metalTexture));
    sceneObjects.push_back(faucetBase);

    // Haste vertical
    Model* faucetSpout = new Model(glm::vec3(-9.8f, 1.14f, -6.0f), glm::vec3(0.0f), glm::vec3(0.04f, 0.24f, 0.04f));
    faucetSpout->addMesh(Geometry::createCylinder(8, metalTexture));
    sceneObjects.push_back(faucetSpout);

    // Bico curvado
    Model* faucetNozzle = new Model(glm::vec3(-9.75f, 1.20f, -6.0f), glm::vec3(90.0f, 0.0f, 90.0f), glm::vec3(0.03f, 0.12f, 0.03f));
    faucetNozzle->addMesh(Geometry::createCylinder(8, metalTexture));
    sceneObjects.push_back(faucetNozzle);

    // === ESPELHO NA PAREDE ===
    
    // Moldura externa (madeira escura)
    Model* mirrorFrame = new Model(glm::vec3(-9.92f, 1.8f, -6.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(1.1f, 1.3f, 0.05f));
    mirrorFrame->addMesh(Geometry::createCube(nullptr));
    sceneObjects.push_back(mirrorFrame);
    size_t mirrorFrameIndex = sceneObjects.size() - 1; // Guarda índice da moldura

    // Superfície do espelho
    Model* mirrorSurface = new Model(glm::vec3(-9.96f, 1.8f, -6.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(1.0f, 1.2f, 0.01f));
    mirrorSurface->addMesh(Geometry::createCube(nullptr));
    sceneObjects.push_back(mirrorSurface);
    size_t mirrorSurfaceIndex = sceneObjects.size() - 1; // Guarda índice da superfície

    // === PRATELEIRA LATERAL ===
    
    Model* bathroomShelf = new Model(glm::vec3(-9.85f, 1.4f, -4.5f), glm::vec3(0.0f), glm::vec3(0.35f, 0.03f, 0.45f));
    bathroomShelf->addMesh(Geometry::createCube(woodTexture));
    sceneObjects.push_back(bathroomShelf);

    // Itens na prateleira
    Model* soapBottle = new Model(glm::vec3(-9.85f, 1.5f, -4.5f), glm::vec3(0.0f), glm::vec3(0.07f, 0.18f, 0.07f));
    soapBottle->addMesh(Geometry::createCylinder(8, nullptr));
    sceneObjects.push_back(soapBottle);

    // ==========================================
    // BANHEIRA
    // ==========================================
    
    // FUNDO da banheira
    Model* bathtubBottom = new Model(glm::vec3(-4.5f, 0.15f, -3.6f), glm::vec3(0.0f), glm::vec3(1.5f, 0.08f, 0.95f));
    bathtubBottom->addMesh(Geometry::createCube(tileTexture));
    sceneObjects.push_back(bathtubBottom);

    // PAREDE LATERAL ESQUERDA
    Model* bathtubWallLeft = new Model(glm::vec3(-5.18f, 0.42f, -3.6f), glm::vec3(0.0f), glm::vec3(0.14f, 0.62f, 0.95f));
    bathtubWallLeft->addMesh(Geometry::createCube(tileTexture));
    sceneObjects.push_back(bathtubWallLeft);

    // PAREDE LATERAL DIREITA
    Model* bathtubWallRight = new Model(glm::vec3(-3.82f, 0.42f, -3.6f), glm::vec3(0.0f), glm::vec3(0.14f, 0.62f, 0.95f));
    bathtubWallRight->addMesh(Geometry::createCube(tileTexture));
    sceneObjects.push_back(bathtubWallRight);

    // PAREDE TRASEIRA/FUNDO
    Model* bathtubWallBack = new Model(glm::vec3(-4.5f, 0.42f, -4.03f), glm::vec3(0.0f), glm::vec3(1.5f, 0.62f, 0.09f));
    bathtubWallBack->addMesh(Geometry::createCube(tileTexture));
    sceneObjects.push_back(bathtubWallBack);

    // PAREDE FRONTAL
    Model* bathtubWallFront = new Model(glm::vec3(-4.5f, 0.42f, -3.17f), glm::vec3(0.0f), glm::vec3(1.5f, 0.62f, 0.09f));
    bathtubWallFront->addMesh(Geometry::createCube(tileTexture));
    sceneObjects.push_back(bathtubWallFront);

    // PÉS DA BANHEIRA
    float bathtubLegX[4] = {-5.15f, -5.15f, -3.85f, -3.85f};
    float bathtubLegZ[4] = {-4.05f, -3.15f, -4.05f, -3.15f};
    
    for (int i = 0; i < 4; i++) {
        Model* bathtubLeg = new Model(
            glm::vec3(bathtubLegX[i], 0.08f, bathtubLegZ[i]), 
            glm::vec3(0.0f), 
            glm::vec3(0.10f, 0.16f, 0.10f)
        );
        bathtubLeg->addMesh(Geometry::createCylinder(8, metalTexture));
        sceneObjects.push_back(bathtubLeg);
    }

    // TORNEIRA DA BANHEIRA
    Model* bathtubFaucet = new Model(glm::vec3(-4.5f, 0.68f, -4.12f), glm::vec3(0.0f), glm::vec3(0.25f, 0.08f, 0.05f));
    bathtubFaucet->addMesh(Geometry::createCube(metalTexture));
    sceneObjects.push_back(bathtubFaucet);

    // Bicos das torneiras
    Model* bathtubHotTap = new Model(glm::vec3(-4.65f, 0.68f, -4.08f), glm::vec3(0.0f), glm::vec3(0.06f, 0.06f, 0.06f));
    bathtubHotTap->addMesh(Geometry::createCylinder(8, metalTexture));
    sceneObjects.push_back(bathtubHotTap);

    Model* bathtubColdTap = new Model(glm::vec3(-4.35f, 0.68f, -4.08f), glm::vec3(0.0f), glm::vec3(0.06f, 0.06f, 0.06f));
    bathtubColdTap->addMesh(Geometry::createCylinder(8, metalTexture));
    sceneObjects.push_back(bathtubColdTap);

    // CHUVEIRINHO DE MÃO
    Model* handShower = new Model(glm::vec3(-3.95f, 0.65f, -3.6f), glm::vec3(0.0f, 0.0f, -30.0f), glm::vec3(0.05f, 0.20f, 0.05f));
    handShower->addMesh(Geometry::createCylinder(8, metalTexture));
    sceneObjects.push_back(handShower);

    // ==========================================
    // ACESSÓRIOS E DETALHES
    // ==========================================
    
    // TOALHEIRO DE PAREDE
    Model* towelRail = new Model(glm::vec3(-3.0f, 1.2f, -3.2f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.70f, 0.03f, 0.03f));
    towelRail->addMesh(Geometry::createCylinder(12, metalTexture));
    sceneObjects.push_back(towelRail);

    // TOALHA PENDURADA
    Model* hangingTowel = new Model(glm::vec3(-3.0f, 1.0f, -3.2f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.65f, 0.40f, 0.02f));
    hangingTowel->addMesh(Geometry::createCube(fabricTexture));
    sceneObjects.push_back(hangingTowel);

    // LIXEIRA
    Model* bathroomTrash = new Model(glm::vec3(-6.8f, 0.25f, -9.7f), glm::vec3(0.0f), glm::vec3(0.22f, 0.50f, 0.22f));
    bathroomTrash->addMesh(Geometry::createCylinder(12, nullptr));
    sceneObjects.push_back(bathroomTrash);

    std::cout << "Cena criada com " << sceneObjects.size() << " objetos!" << std::endl;

    // ========== LOOP DE RENDERIZAÇÃO ==========
    while (!glfwWindowShouldClose(window)) {
        // Calcula deltaTime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Processa entrada
        processInput(window);

        // Renderização
        glClearColor(0.3f, 0.3f, 0.35f, 1.0f); // Fundo cinza claro
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Ativa shader
        shader.use();

        // Define matrizes de projeção e visualização
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), 
                                                (float)SCR_WIDTH / (float)SCR_HEIGHT, 
                                                0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();

        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // Define uniforms de iluminação globais
        shader.setVec3("lightPos", lightPos);
        shader.setVec3("lightColor", glm::vec3(1.5f, 1.5f, 1.5f));
        shader.setVec3("viewPos", camera.position);

        // Parâmetros de iluminação Phong
        shader.setFloat("ambientStrength", 0.8f); // MUITA luz ambiente para ver tudo
        shader.setFloat("specularStrength", 0.5f);
        shader.setInt("shininess", 32);

        // Desenha todos os objetos com cores variadas
        glm::vec3 colors[] = {
            glm::vec3(0.8f, 0.6f, 0.4f), // Marrom (madeira)
            glm::vec3(0.9f, 0.9f, 0.85f), // Bege (parede)
            glm::vec3(0.2f, 0.5f, 0.8f),  // Azul
            glm::vec3(0.8f, 0.2f, 0.2f),  // Vermelho
            glm::vec3(0.2f, 0.8f, 0.3f),  // Verde
            glm::vec3(0.9f, 0.8f, 0.5f),  // Amarelo
            glm::vec3(0.6f, 0.4f, 0.8f),  // Roxo
            glm::vec3(0.5f, 0.5f, 0.5f),  // Cinza
        };
        
        glm::vec3 azulEspelho = glm::vec3(0.1f, 0.3f, 0.9f); // Azul vibrante para o espelho
        
        for (size_t i = 0; i < sceneObjects.size(); i++) {
            // Espelho: cor azul específica
            if (i == mirrorFrameIndex || i == mirrorSurfaceIndex) {
                shader.setVec3("objectColor", azulEspelho);
            } else {
                // Outros objetos: cor cíclica
                shader.setVec3("objectColor", colors[i % 8]);
            }
            sceneObjects[i]->draw(shader);
        }

        // Troca buffers e processa eventos
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ========== LIMPEZA ==========
    // Deleta objetos
    for (Model* obj : sceneObjects) {
        delete obj;
    }

    // Deleta texturas
    delete woodTexture;
    delete wallTexture;
    delete fabricTexture;
    delete tileTexture;
    delete metalTexture;

    glfwTerminate();
    return 0;
}

