#include <glad/glad.h>

#include "engine/window.hpp"
#include "engine/shader.hpp"

#include "engine/texture.hpp"
#include "engine/geometry/cube.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

void handleInput() {/*Intentionally Left BLank*/ }

void render(const Geometry& geom, const Shader& shader, Texture& tex) {
    glClear(GL_COLOR_BUFFER_BIT);

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.5f, 0.8f, -0.5f),
        glm::vec3(-0.4f, -0.2f, -0.0f),
        glm::vec3(0.9f, 0.3f, -0.8f),
        glm::vec3(0.7f, -0.5f, 0.0f)
    };

    glm::mat4 model = glm::mat4(1.0f); //matriz identidad diagonal
    model = glm::translate(model, cubePositions[0]);
    model = glm::scale(model, glm::vec3(1000.0f, 0.1f, 1000.0f));

    glm::mat4 view = glm::mat4(1.0f); //matriz identidad diagonal
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    shader.use();

    tex.use(shader, "tex", 0);

    shader.set("model", model);
    shader.set("view", view);
    shader.set("proj", proj);  //TODO const mat4

    geom.render();

    model = glm::mat4(1.0f); //matriz identidad diagonal
    model = glm::translate(model, cubePositions[1]);
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    model = glm::rotate(model, static_cast<float>(glfwGetTime())* glm::radians(20.0f), glm::vec3(0.5f, 1.0f, 0.0f));

    view = glm::mat4(1.0f); //matriz identidad diagonal
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

    proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    shader.use();

    tex.use(shader, "tex", 0);

    shader.set("model", model);
    shader.set("view", view);
    shader.set("proj", proj);

    geom.render();

    model = glm::mat4(1.0f); //matriz identidad diagonal
    model = glm::translate(model, cubePositions[2]);
    model = glm::scale(model, glm::vec3(0.5f, 0.8f, 0.5f));
    //model = glm::rotate(model, static_cast<float>(glfwGetTime())* glm::radians(20.0f), glm::vec3(0.5f, 1.0f, 0.0f));

    view = glm::mat4(1.0f); //matriz identidad diagonal
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.5f));

    proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    shader.use();

    tex.use(shader, "tex", 0);

    shader.set("model", model);
    shader.set("view", view);
    shader.set("proj", proj);

    geom.render();

    model = glm::mat4(1.0f); //matriz identidad diagonal
    model = glm::translate(model, cubePositions[3]);
    model = glm::scale(model, glm::vec3(0.33f, 0.2f, 0.2f));
    model = glm::rotate(model, static_cast<float>(glfwGetTime())* glm::radians(-33.0f), glm::vec3(-0.7f, 0.5f, 0.3f));

    view = glm::mat4(1.0f); //matriz identidad diagonal
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

    proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    shader.use();

    tex.use(shader, "tex", 0);

    shader.set("model", model);
    shader.set("view", view);
    shader.set("proj", proj);

    geom.render();

    model = glm::mat4(1.0f); //matriz identidad diagonal
    model = glm::translate(model, cubePositions[4]);
    model = glm::scale(model, glm::vec3(0.5f, 0.25f, 0.1f));
    

    view = glm::mat4(1.0f); //matriz identidad diagonal
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));

    proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    shader.use();

    tex.use(shader, "tex", 0);

    shader.set("model", model);
    shader.set("view", view);
    shader.set("proj", proj);

    geom.render();
}



int main(int, char* []) {
    Window* window = Window::instance();

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    const Shader shader("../projects/EJ05_04/vertex.vs", "../projects/EJ05_04/fragment.fs");
    const Cube cube(1.0f);

    Texture tex("../assets/textures/blue_blocks.jpg", Texture::Format::RGB);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    while (window->alive()) {

        handleInput();
        render(cube, shader, tex);

        window->frame();
    }

    return 0;
}