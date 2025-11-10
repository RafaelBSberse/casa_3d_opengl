#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <cmath>
#include "Mesh.h"

// Definição de M_PI para Windows
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Namespace com funções para gerar formas básicas 3D
namespace Geometry {

    // ============ CUBO ============
    // Gera um cubo unitário (1x1x1) centrado na origem
    Mesh* createCube(Texture* texture = nullptr) {
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        // Vértices do cubo com normais apontando para fora
        // Face frontal (Z+)
        vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)));
        vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)));

        // Face traseira (Z-)
        vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)));
        vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)));

        // Face superior (Y+)
        vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)));
        vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)));

        // Face inferior (Y-)
        vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)));
        vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)));

        // Face direita (X+)
        vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)));
        vertices.push_back(Vertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)));

        // Face esquerda (X-)
        vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)));
        vertices.push_back(Vertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)));

        // Índices para as 6 faces (2 triângulos por face)
        GLuint faceIndices[] = {
            0, 1, 2, 2, 3, 0,       // Frontal
            4, 5, 6, 6, 7, 4,       // Traseira
            8, 9, 10, 10, 11, 8,    // Superior
            12, 13, 14, 14, 15, 12, // Inferior
            16, 17, 18, 18, 19, 16, // Direita
            20, 21, 22, 22, 23, 20  // Esquerda
        };

        for (int i = 0; i < 36; i++) {
            indices.push_back(faceIndices[i]);
        }

        return new Mesh(vertices, indices, texture);
    }

    // ============ PRISMA TRIANGULAR ============
    // Gera um prisma triangular (base triangular) ao longo do eixo Z
    Mesh* createTriangularPrism(Texture* texture = nullptr) {
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        float height = 1.0f;
        float base = 1.0f;

        // Face frontal (triângulo Z+)
        vertices.push_back(Vertex(glm::vec3(0.0f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.5f, 1.0f)));
        vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)));

        // Face traseira (triângulo Z-)
        vertices.push_back(Vertex(glm::vec3(0.0f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.5f, 1.0f)));
        vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)));

        // Face inferior (retangular)
        vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)));
        vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)));
        vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)));

        // Face lateral esquerda (inclinada)
        glm::vec3 norm1 = glm::normalize(glm::vec3(-1.0f, 1.0f, 0.0f));
        vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, 0.5f), norm1, glm::vec2(0.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(0.0f, 0.5f, 0.5f), norm1, glm::vec2(0.5f, 1.0f)));
        vertices.push_back(Vertex(glm::vec3(0.0f, 0.5f, -0.5f), norm1, glm::vec2(1.0f, 1.0f)));
        vertices.push_back(Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), norm1, glm::vec2(1.0f, 0.0f)));

        // Face lateral direita (inclinada)
        glm::vec3 norm2 = glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f));
        vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, 0.5f), norm2, glm::vec2(0.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(0.5f, -0.5f, -0.5f), norm2, glm::vec2(1.0f, 0.0f)));
        vertices.push_back(Vertex(glm::vec3(0.0f, 0.5f, -0.5f), norm2, glm::vec2(1.0f, 1.0f)));
        vertices.push_back(Vertex(glm::vec3(0.0f, 0.5f, 0.5f), norm2, glm::vec2(0.0f, 1.0f)));

        // Índices
        GLuint prismIndices[] = {
            0, 1, 2,          // Face frontal
            3, 4, 5,          // Face traseira
            6, 7, 8, 8, 9, 6, // Face inferior
            10, 11, 12, 12, 13, 10, // Lateral esquerda
            14, 15, 16, 16, 17, 14  // Lateral direita
        };

        for (int i = 0; i < 24; i++) {
            indices.push_back(prismIndices[i]);
        }

        return new Mesh(vertices, indices, texture);
    }

    // ============ CILINDRO ============
    // Gera um cilindro ao longo do eixo Y
    Mesh* createCylinder(int segments = 32, Texture* texture = nullptr) {
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        float radius = 0.5f;
        float height = 1.0f;

        // Vértices da base inferior (Y = -0.5)
        for (int i = 0; i <= segments; i++) {
            float angle = (float)i / (float)segments * 2.0f * M_PI;
            float x = cos(angle) * radius;
            float z = sin(angle) * radius;

            vertices.push_back(Vertex(
                glm::vec3(x, -0.5f, z),
                glm::vec3(x, 0.0f, z), // Normal lateral
                glm::vec2((float)i / (float)segments, 0.0f)
            ));
        }

        // Vértices da base superior (Y = 0.5)
        for (int i = 0; i <= segments; i++) {
            float angle = (float)i / (float)segments * 2.0f * M_PI;
            float x = cos(angle) * radius;
            float z = sin(angle) * radius;

            vertices.push_back(Vertex(
                glm::vec3(x, 0.5f, z),
                glm::vec3(x, 0.0f, z), // Normal lateral
                glm::vec2((float)i / (float)segments, 1.0f)
            ));
        }

        // Índices da superfície lateral
        for (int i = 0; i < segments; i++) {
            indices.push_back(i);
            indices.push_back(i + segments + 1);
            indices.push_back(i + 1);

            indices.push_back(i + 1);
            indices.push_back(i + segments + 1);
            indices.push_back(i + segments + 2);
        }

        // Centro da base inferior
        int centerBottom = vertices.size();
        vertices.push_back(Vertex(glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.5f, 0.5f)));

        // Triângulos da base inferior
        for (int i = 0; i < segments; i++) {
            float angle = (float)i / (float)segments * 2.0f * M_PI;
            float x = cos(angle) * radius;
            float z = sin(angle) * radius;

            vertices.push_back(Vertex(
                glm::vec3(x, -0.5f, z),
                glm::vec3(0.0f, -1.0f, 0.0f),
                glm::vec2(0.5f + cos(angle) * 0.5f, 0.5f + sin(angle) * 0.5f)
            ));
        }

        for (int i = 0; i < segments; i++) {
            indices.push_back(centerBottom);
            indices.push_back(centerBottom + i + 1);
            indices.push_back(centerBottom + ((i + 1) % segments) + 1);
        }

        // Centro da base superior
        int centerTop = vertices.size();
        vertices.push_back(Vertex(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.5f, 0.5f)));

        // Triângulos da base superior
        for (int i = 0; i < segments; i++) {
            float angle = (float)i / (float)segments * 2.0f * M_PI;
            float x = cos(angle) * radius;
            float z = sin(angle) * radius;

            vertices.push_back(Vertex(
                glm::vec3(x, 0.5f, z),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec2(0.5f + cos(angle) * 0.5f, 0.5f + sin(angle) * 0.5f)
            ));
        }

        for (int i = 0; i < segments; i++) {
            indices.push_back(centerTop);
            indices.push_back(centerTop + ((i + 1) % segments) + 1);
            indices.push_back(centerTop + i + 1);
        }

        return new Mesh(vertices, indices, texture);
    }

    // ============ ESFERA ============
    // Gera uma esfera usando subdivisão de latitude/longitude
    Mesh* createSphere(int latSegments = 32, int lonSegments = 32, Texture* texture = nullptr) {
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        float radius = 0.5f;

        // Gera vértices
        for (int lat = 0; lat <= latSegments; lat++) {
            float theta = (float)lat * M_PI / (float)latSegments;
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            for (int lon = 0; lon <= lonSegments; lon++) {
                float phi = (float)lon * 2.0f * M_PI / (float)lonSegments;
                float sinPhi = sin(phi);
                float cosPhi = cos(phi);

                float x = cosPhi * sinTheta;
                float y = cosTheta;
                float z = sinPhi * sinTheta;

                glm::vec3 position(x * radius, y * radius, z * radius);
                glm::vec3 normal(x, y, z); // Normal aponta para fora do centro
                glm::vec2 texCoord((float)lon / (float)lonSegments, (float)lat / (float)latSegments);

                vertices.push_back(Vertex(position, normal, texCoord));
            }
        }

        // Gera índices
        for (int lat = 0; lat < latSegments; lat++) {
            for (int lon = 0; lon < lonSegments; lon++) {
                int first = lat * (lonSegments + 1) + lon;
                int second = first + lonSegments + 1;

                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }

        return new Mesh(vertices, indices, texture);
    }

} // namespace Geometry

#endif
