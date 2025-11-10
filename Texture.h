#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <string>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

// Classe para carregar e gerenciar texturas
class Texture {
public:
    GLuint ID;
    std::string type;
    std::string path;

    // Construtor padrão
    Texture() : ID(0) {}

    // Carrega uma textura de um arquivo
    Texture(const char* filepath, bool flip = true) {
        glGenTextures(1, &ID);
        path = filepath;

        int width, height, nrChannels;

        // Inverte a textura verticalmente (OpenGL espera origem no canto inferior esquerdo)
        stbi_set_flip_vertically_on_load(flip);

        unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);

        if (data) {
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, ID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            // Define parâmetros de wrapping
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            // Define parâmetros de filtragem
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            std::cout << "Textura carregada com sucesso: " << filepath << std::endl;
        }
        else {
            std::cout << "Falha ao carregar textura: " << filepath << std::endl;
        }

        stbi_image_free(data);
    }

    // Bind da textura
    void bind(GLuint unit = 0) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, ID);
    }

    // Unbind da textura
    void unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};

#endif
