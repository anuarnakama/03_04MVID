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

glm::vec3 solidQuadPosition = glm::vec3(0.0f, 0.0f, 2.0f);

glm::vec3 quadPositions[] = {
    glm::vec3(0.0f, 0.0f, 1.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, -1.0f)
};


glm::vec4 quadColors[] = {
    glm::vec4(1, 0, 0, 0.75f),
    glm::vec4(0, 1, 0, 0.50f),
    glm::vec4(0, 0, 1, 0.25f)
};

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

void render(const Geometry& floor, const Shader& s_phong, const Shader& s_alpha,
    const Texture& t_bricks_albedo, const Texture& t_bricks_specular) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), 800.0f / 600.0f, 0.1f, 100.0f);

    s_phong.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
    s_phong.set("model", model);
    s_phong.set("view", view);
    s_phong.set("proj", proj);

    glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    s_phong.set("normalMat", normalMat);

    s_phong.set("viewPos", camera.getPosition());

    s_phong.set("light.direction", -0.2f, -1.0f, -0.3f);
    s_phong.set("light.ambient", 0.1f, 0.1f, 0.1f);
    s_phong.set("light.diffuse", 0.5f, 0.5f, 0.5f);
    s_phong.set("light.specular", 1.0f, 1.0f, 1.0f);

    t_bricks_albedo.use(s_phong, "material.diffuse", 0);
    t_bricks_specular.use(s_phong, "material.specular", 1);
    s_phong.set("material.shininess", 32);

    floor.render();

    glDisable(GL_CULL_FACE);

    model = glm::mat4(1.0f);
    model = glm::translate(model, solidQuadPosition);
    s_phong.set("model", model);

    normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
    s_phong.set("normalMat", normalMat);

    floor.render();

    s_alpha.use();
    s_alpha.set("view", view);
    s_alpha.set("proj", proj);
    s_alpha.set("viewPos", camera.getPosition());
    s_alpha.set("light.direction", -0.2f, -1.0f, -0.3f);
    s_alpha.set("light.ambient", 0.1f, 0.1f, 0.1f);
    s_alpha.set("light.diffuse", 0.5f, 0.5f, 0.5f);
    s_alpha.set("light.specular", 1.0f, 1.0f, 1.0f);
    s_alpha.set("material.shininess", 32);


    glEnable(GL_BLEND);
    int numQuads = sizeof(quadPositions) / sizeof(quadPositions[0]);
    for (int i = 0; i < numQuads; ++i) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, quadPositions[i]);
        s_alpha.set("model", model);

        glm::mat3 normalMat = glm::inverse(glm::transpose(glm::mat3(model)));
        s_alpha.set("normalMat", normalMat);

        s_alpha.set("material.color", quadColors[i]);

        floor.render();
    }
    glDisable(GL_BLEND);

    glEnable(GL_CULL_FACE);
}

int main(int, char* []) {
    Window* window = Window::instance();

    glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

    const Shader s_phong("../projects/EJ11_02/phong.vs", "../projects/EJ11_02/blinn.fs");
    const Shader s_alpha("../projects/EJ11_02/phong.vs", "../projects/EJ11_02/blinn_alpha.fs");
    const Texture t_bricks_albedo("../assets/textures/bricks_albedo.png", Texture::Format::RGB);
    const Texture t_bricks_specular("../assets/textures/bricks_specular.png", Texture::Format::RGB);
    const Texture t_wood_albedo("../assets/textures/wood.jpg", Texture::Format::RGB);
    const Texture t_wood_specular("../assets/textures/bricks_specular.png", Texture::Format::RGB);
    const Texture t_grass_albedo("../assets/textures/grass.png", Texture::Format::RGBA);
    const Cube cube(1.0f);
    const Quad quad(1.0f);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
        render(quad, s_phong, s_alpha, t_bricks_albedo, t_bricks_specular);
        window->frame();
    }

    return 0;
}
