#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"
#include "Texture.h"

// Estrutura de vértice com posição, normal e coordenadas de textura
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;

    Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 tex)
        : position(pos), normal(norm), texCoords(tex) {}
};

// Classe Mesh - gerencia VAO, VBO, IBO de uma malha
class Mesh {
public:
    // Dados da malha
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    Texture* texture;

    // Dados de renderização
    GLuint VAO, VBO, EBO;

    // Construtor
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, Texture* tex = nullptr) {
        this->vertices = vertices;
        this->indices = indices;
        this->texture = tex;

        setupMesh();
    }

    // Renderiza a malha
    void draw(Shader& shader) {
        // Bind da textura se existir
        if (texture != nullptr) {
            texture->bind(0);
            shader.setInt("texture1", 0);
            shader.setBool("useTexture", true);
        }
        else {
            shader.setBool("useTexture", false);
        }

        // Desenha a malha
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Volta ao estado padrão
        glActiveTexture(GL_TEXTURE0);
    }

    // Destrutor
    ~Mesh() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

private:
    // Inicializa todos os buffers e arrays
    void setupMesh() {
        // Cria buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // Carrega dados no vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        // Carrega dados no element buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

        // Define os ponteiros de atributos de vértice

        // Posições dos vértices
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // Normais dos vértices
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        // Coordenadas de textura dos vértices
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

        glBindVertexArray(0);
    }
};

#endif
