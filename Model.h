#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Mesh.h"
#include "Shader.h"

// Classe Model - agrupa múltiplas meshes e gerencia transformações
class Model {
public:
    std::vector<Mesh*> meshes;
    glm::vec3 position;
    glm::vec3 rotation; // Em graus (x, y, z)
    glm::vec3 scale;

    // Construtor
    Model(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scl = glm::vec3(1.0f))
        : position(pos), rotation(rot), scale(scl) {}

    // Adiciona uma mesh ao modelo
    void addMesh(Mesh* mesh) {
        meshes.push_back(mesh);
    }

    // Calcula a matriz de modelo
    glm::mat4 getModelMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);

        // Translação
        model = glm::translate(model, position);

        // Rotação (aplicada na ordem X, Y, Z)
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        // Escala
        model = glm::scale(model, scale);

        return model;
    }

    // Desenha o modelo
    void draw(Shader& shader) {
        // Define a matriz de modelo
        shader.setMat4("model", getModelMatrix());

        // Desenha todas as meshes
        for (Mesh* mesh : meshes) {
            mesh->draw(shader);
        }
    }

    // Destrutor
    ~Model() {
        // Nota: meshes são gerenciadas externamente, não deletamos aqui
    }
};

#endif
