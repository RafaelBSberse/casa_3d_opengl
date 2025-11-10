#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Shader.h>
#include <Application.h>
#include <Cube.h>
#include <Banrisul.h>
#include <Texture.h>
#include <iostream>

int main() {
    // Cria janela e inicializa OpenGL
    Application app(1024, 768, "Cubos OOP");
    if (!app.init()) return -1;

    // Shader
    Shader shader("vertex.glsl", "fragment.glsl");
    shader.use();

    // Carrega texturas
    Texture tex1("pedra-28.jpg");
    Texture tex2("opengl.png");  // logo OpenGL com alpha

    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    // Cria cubos com posições diferentes
    Cube cube2(glm::vec3(2.0f, 0.0f, -2.5f));
    Cube cube3(glm::vec3(-2.0f, 1.0f, -4.0f));
    Cube cube4(glm::vec3(-3.0f, -1.0f, 3.0f));
    Cube cube5(glm::vec3(-4.0f, 0.0f, 0.0f));

    Banrisul banri(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f);
    banri.scale = glm::vec3(2.5f, 2.5f, 2.5f);

    // Ativa depth test
    glEnable(GL_DEPTH_TEST);

    // Loop principal
    while (!glfwWindowShouldClose(app.getWindow())) {
        // Processa input
        if (glfwGetKey(app.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(app.getWindow(), true);

        // Limpa tela e depth buffer
        //glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClearColor(0.7f, 0.5f, 0.1f, 1.0f);
        //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glm::mat4 model = glm::mat4(1.0f);
        float angle = 20.0f;
        model = glm::rotate(model, (angle * (float) glfwGetTime()) / 20, glm::vec3(0.0f, 0.3f, 0.0f));
        //model = glm::rotate(model, -60.0f, glm::vec3(0.0f, 0.3f, 0.0f));

        shader.setMat4("model", model);

        // Configura view e projection
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                800.0f / 600.0f,
                                                0.1f, 100.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.0f, -8.0f));
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // Bind texturas
        tex1.bind(0);
        tex2.bind(1);


        // Desnha logo banrisul

        banri.draw(shader, model);

        // Desenha cubos


        model = glm::rotate(model, (angle * (float) glfwGetTime()) / 20, glm::vec3(0.5f, -0.2f, 0.5f));
        shader.setMat4("model", model);
        cube2.draw(shader, model);

        model = glm::rotate(model, (angle * (float) glfwGetTime()) / 5, glm::vec3(1.5f, 4.2f, 0.1f));
        shader.setMat4("model", model);
        cube3.draw(shader, model);

        model = glm::rotate(model, (-angle * (float) glfwGetTime()) / 40, glm::vec3(-0.8f, 2.1f, -2.0f));
        shader.setMat4("model", model);
        cube4.draw(shader, model);

        model = glm::rotate(model, (angle * (float) glfwGetTime()) / 40, glm::vec3(-0.5f, -0.2f, 1.45f));
        shader.setMat4("model", model);
        cube4.draw(shader, model);

        model = glm::mat4(1.0f);
        model = glm::rotate(model, (angle * (float) glfwGetTime()) / -40, glm::vec3(0.0f, 1.0f, 0.0f));
        cube5.draw(shader, model);

        // Swap buffers e eventos
        glfwSwapBuffers(app.getWindow());
        glfwPollEvents();
    }

    return 0;
}
