#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Classe Camera baseada na Aula 13 - Implementa câmera em primeira pessoa
class Camera {
public:
    // Vetores da câmera
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    // Ângulos de Euler (em graus)
    float yaw;    // Rotação no eixo Y (esquerda/direita)
    float pitch;  // Rotação no eixo X (cima/baixo)

    // Opções da câmera
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    // Construtor com valores padrão
    Camera(glm::vec3 pos = glm::vec3(0.0f, 1.7f, 5.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f,
           float pitch = 0.0f)
        : front(glm::vec3(0.0f, 0.0f, -1.0f)),
          movementSpeed(2.5f),
          mouseSensitivity(0.1f),
          zoom(45.0f)
    {
        this->position = pos;
        this->worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;
        updateCameraVectors();
    }

    // Retorna a matriz de visualização calculada usando glm::lookAt
    glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    // Processa entrada do teclado - movimento WASD
    void processKeyboard(int direction, float deltaTime) {
        float velocity = movementSpeed * deltaTime;

        if (direction == 0) // W - Frente
            position += front * velocity;
        if (direction == 1) // S - Trás
            position -= front * velocity;
        if (direction == 2) // A - Esquerda
            position -= right * velocity;
        if (direction == 3) // D - Direita
            position += right * velocity;
        if (direction == 4) // Space - Cima
            position += worldUp * velocity;
        if (direction == 5) // Left Shift - Baixo
            position -= worldUp * velocity;
    }

    // Processa movimento do mouse - look around
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // Limita o pitch para evitar flip da câmera
        if (constrainPitch) {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        // Atualiza os vetores front, right e up
        updateCameraVectors();
    }

    // Processa scroll do mouse - zoom
    void processMouseScroll(float yoffset) {
        zoom -= yoffset;
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > 45.0f)
            zoom = 45.0f;
    }

private:
    // Calcula o vetor front a partir dos ângulos de Euler atualizados
    // Baseado na lógica da função viraCamera da Aula 13
    void updateCameraVectors() {
        // Calcula o novo vetor front
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);

        // Re-calcula os vetores right e up
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
};

#endif
