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
#include "engine/geometry/quad.hpp"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 lightPos(4.0f, 1.0f, 0.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

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

void render(const Geometry& object, const Geometry& object2, const Geometry& object3, 
    const Geometry& light, const Shader& s_phong, const Shader& s_light, const Shader& s_gouraud, const Shader& s_flat) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);

    //Luz
    s_light.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.25f));
    s_light.set("model", model);
    s_light.set("view", view);
    s_light.set("proj", proj);
    s_light.set("lightColor", lightColor);
    light.render();


    //Esfera 1
    s_phong.use();
    model = glm::mat4(1.0f);
    s_phong.set("model", model);
    s_phong.set("view", view);
    s_phong.set("proj", proj);

    glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    s_phong.set("normalMat", normalMat);

    s_phong.set("objectColor", glm::vec3(0.6f, 0.5f, 0.2f));
    s_phong.set("lightColor", lightColor);

    s_phong.set("ambientStrength", 0.2f);
    s_phong.set("lightPos", lightPos);

    s_phong.set("viewPos", camera.getPosition());
    s_phong.set("shininess", 64);
    s_phong.set("specularStrength", 0.6f);

    object.render();

    //Esfera 2
    glm::vec3 objetc2Pos(4.0f, 1.0f, 4.0f);
    s_gouraud.use();
    model = glm::mat4(1.0f);
    model = glm::translate(model, objetc2Pos);
    s_gouraud.set("model", model);
    s_gouraud.set("view", view);
    s_gouraud.set("proj", proj);

    normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    s_gouraud.set("normalMat", normalMat);

    s_gouraud.set("objectColor", glm::vec3(0.6f, 0.9f, 0.1f));
    s_gouraud.set("lightColor", lightColor);

    s_gouraud.set("ambientStrength", 0.2f);
    s_gouraud.set("lightPos", lightPos);

    s_gouraud.set("viewPos", camera.getPosition());
    s_gouraud.set("shininess", 64);
    s_gouraud.set("specularStrength", 0.6f);

    object2.render();

    //Esfera 3
    glm::vec3 objetc3Pos(-4.0f, 1.0f, -4.0f);
    s_flat.use();
    model = glm::mat4(1.0f);
    model = glm::translate(model, objetc3Pos);
    s_flat.set("model", model);
    s_flat.set("view", view);
    s_flat.set("proj", proj);

    normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    s_flat.set("normalMat", normalMat);

    s_flat.set("objectColor", glm::vec3(0.6f, 0.6f, 0.2f));
    s_flat.set("lightColor", lightColor);

    s_flat.set("ambientStrength", 0.2f);
    s_flat.set("lightPos", lightPos);

    s_flat.set("viewPos", camera.getPosition());
    s_flat.set("shininess", 64);
    s_flat.set("specularStrength", 0.6f);

    object3.render();
}

int main(int, char* []) {
    Window* window = Window::instance();

    glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

    const Shader s_phong("../projects/EJ07_05/phong.vs", "../projects/EJ07_05/blinn.fs");
    const Shader s_gouraud("../projects/EJ07_04/gouraud.vs", "../projects/EJ07_04/gouraud.fs");
    const Shader s_light("../projects/EJ07_05/light.vs", "../projects/EJ07_05/light.fs");
    const Shader s_flat("../projects/EJ07_05/flat.vs", "../projects/EJ07_05/flat.fs");
    const Sphere sphere(1.0f, 50, 50);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    Input::instance()->setKeyPressedCallback(onKeyPress);
    Input::instance()->setMouseMoveCallback(onMouseMoved);
    Input::instance()->setScrollMoveCallback(onScrollMoved);

    while (window->alive()) {
        const float currentFrame = glfwGetTime();
        const float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        handleInput(deltaTime);
        render(sphere, sphere, sphere, sphere, s_phong, s_light, s_gouraud, s_flat);
        window->frame();
    }

    return 0;
}