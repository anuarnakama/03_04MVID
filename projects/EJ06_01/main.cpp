#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/camera.hpp"
#include "engine/geometry/cube.hpp"
#include "engine/input.hpp"
#include "engine/shader.hpp"
#include "engine/texture.hpp"
#include "engine/window.hpp"
#include "engine/geometry/sphere.hpp"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastFrame = 0.0f;
float lastX, lastY;
bool firstMouse = true;

void handleInput(float dt) {
    Input* input = Input::instance();

    if (input->isKeyPressed(GLFW_KEY_W)) {
        camera.handleKeyboard(Camera::Movement::Forward, dt);
    }
    if (input->isKeyPressed(GLFW_KEY_S)) {
        camera.handleKeyboard(Camera::Movement::Backward, dt);
    }
    if (input->isKeyPressed(GLFW_KEY_A)) {
        camera.handleKeyboard(Camera::Movement::Left, dt);
    }
    if (input->isKeyPressed(GLFW_KEY_D)) {
        camera.handleKeyboard(Camera::Movement::Right, dt);
    }
}

void onKeyPress(int key, int action) {
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        Window::instance()->setCaptureMode(true);
    }

    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        Window::instance()->setCaptureMode(false);
    }
}

void onMouseMoved(float x, float y) {
    if (firstMouse) {
        firstMouse = false;
        lastX = x;
        lastY = y;
    }

    const float xoffset = x - lastX;
    const float yoffset = lastY - y;
    lastX = x;
    lastY = y;

    camera.handleMouseMovement(xoffset, yoffset);
}

void onScrollMoved(float x, float y) {
    camera.handleMouseScroll(y);
}

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

    glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);

    shader.use();

    tex.use(shader, "tex", 0);

    shader.set("model", model);
    shader.set("view", camera.getViewMatrix());
    shader.set("proj", proj);  //TODO const mat4

    geom.render();

    model = glm::mat4(1.0f); //matriz identidad diagonal
    model = glm::translate(model, cubePositions[1]);
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    model = glm::rotate(model, static_cast<float>(glfwGetTime())* glm::radians(20.0f), glm::vec3(0.5f, 1.0f, 0.0f));

    view = glm::mat4(1.0f); //matriz identidad diagonal
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

    proj = glm::perspective(glm::radians(camera.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);

    shader.use();

    tex.use(shader, "tex", 0);

    shader.set("model", model);
    shader.set("view", camera.getViewMatrix());
    shader.set("proj", proj);

    geom.render();

    model = glm::mat4(1.0f); //matriz identidad diagonal
    model = glm::translate(model, cubePositions[2]);
    model = glm::scale(model, glm::vec3(0.5f, 0.8f, 0.5f));
    //model = glm::rotate(model, static_cast<float>(glfwGetTime())* glm::radians(20.0f), glm::vec3(0.5f, 1.0f, 0.0f));

    view = glm::mat4(1.0f); //matriz identidad diagonal
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.5f));

    proj = glm::perspective(glm::radians(camera.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);

    shader.use();

    tex.use(shader, "tex", 0);

    shader.set("model", model);
    shader.set("view", camera.getViewMatrix());
    shader.set("proj", proj);

    geom.render();

    model = glm::mat4(1.0f); //matriz identidad diagonal
    model = glm::translate(model, cubePositions[3]);
    model = glm::scale(model, glm::vec3(0.33f, 0.2f, 0.2f));
    model = glm::rotate(model, static_cast<float>(glfwGetTime())* glm::radians(-33.0f), glm::vec3(-0.7f, 0.5f, 0.3f));

    view = glm::mat4(1.0f); //matriz identidad diagonal
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

    proj = glm::perspective(glm::radians(camera.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);

    shader.use();

    tex.use(shader, "tex", 0);

    shader.set("model", model);
    shader.set("view", camera.getViewMatrix());
    shader.set("proj", proj);

    geom.render();

    model = glm::mat4(1.0f); //matriz identidad diagonal
    model = glm::translate(model, cubePositions[4]);
    model = glm::scale(model, glm::vec3(0.5f, 0.25f, 0.1f));


    view = glm::mat4(1.0f); //matriz identidad diagonal
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));

    proj = glm::perspective(glm::radians(camera.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);

    shader.use();

    tex.use(shader, "tex", 0);

    shader.set("model", model);
    shader.set("view", camera.getViewMatrix());
    shader.set("proj", proj);

    geom.render();
}

int main(int, char* []) {
    Window* window = Window::instance();

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    const Shader shader("../projects/AG06/vertex.vs", "../projects/AG06/fragment.fs");
    const Cube cube(1.0f);
    //const Sphere sphere(1.0f, 50, 50);

    Texture tex("../assets/textures/blue_blocks.jpg", Texture::Format::RGB);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    Input::instance()->setKeyPressedCallback(onKeyPress);
    Input::instance()->setMouseMoveCallback(onMouseMoved);
    Input::instance()->setScrollMoveCallback(onScrollMoved);

    while (window->alive()) {
        const float currentFrame = glfwGetTime();
        const float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        handleInput(deltaTime);
        render(cube, shader, tex);
        window->frame();
    }

    return 0;
}