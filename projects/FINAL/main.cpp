#include "engine/camera.hpp"
#include "engine/geometry/cube.hpp"
#include "engine/input.hpp"
#include "engine/shader.hpp"
#include "engine/texture.hpp"
#include "engine/window.hpp"
#include "engine/geometry/sphere.hpp"
#include "engine/geometry/quad.hpp"
#include "engine/model.hpp"
#include "engine/light.hpp"
#include "engine/fbo.hpp"
#include "engine/material.hpp"
#include "engine/drawable.hpp"
#include "engine/block.hpp"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <ctime>

// Configuracion de parametros generales ********************************************************************************

//Creacion de la camara
Camera camera(glm::vec3(0.0f, 0.2f, 12.0f));

//Parametros luz ambiental
DirectionalLight dirLight(
    glm::vec3(-1.0f, -0.5f, -3.0f), //Direction
    glm::vec3(1.0f, 0.5f, 3.0f),    //Position
    glm::vec3(0.1f, 0.1f, 0.1f),    //Ambient
    glm::vec3(0.1f, 0.1f, 0.1f),    //Diffuse
    glm::vec3(0.2f, 0.2f, 0.2f)     //Specular
);

//Paramentros para sombras
const uint32_t k_shadow_height = 1024;
const uint32_t k_shadow_width = 1024;
const float k_shadow_near = 1.0f;
const float k_shadow_far = 7.5f;
FBO fboShadow(k_shadow_width, k_shadow_height);

//Tamaño del fondo
glm::vec3 BckSize(12.0f, 10.0f, 10.0f);

//Parametros de la barra
glm::vec3 barPos(0.0f, -3.8f, 0.0f);
glm::vec3 barSize(1.0f, 0.2f, 0.2f);
const float k_barSpeed = 6.0f;
uint32_t score = 0;

// Vidas del jugador
uint16_t lifes = 3;

//Estado del juego
bool isPlaying = false;

//Parametros de la bola
glm::vec3 ballPos(barPos.x, barPos.y + 0.3f, barPos.z);
float ballRadius = 0.1f;
const float ballSpeedX = 4.0f;
const float ballSpeedY = 4.0f;
glm::vec2 ballSpeed(ballSpeedX, ballSpeedY);

//Parametros de los bloques
glm::vec3 blockPos(0.0f, 2.5f, 0.0f);
glm::vec3 blockSize(0.5f, 0.2f, 0.2f);
const uint32_t numOfBlocks = 21;
const uint32_t numOfRows = 5;
Block blocks[numOfRows][numOfBlocks];

//PointLights: una ambiental, una en la bola y otra en la barra
uint16_t numberPointLights = 3;
PointLight* pointLight = new PointLight[numberPointLights]
{
    //PointLight
    {glm::vec3(1.0f, 0.5f, 3.0f),   //Position
    glm::vec3(0.3f, 0.3f, 0.3f),    //Ambient
    glm::vec3(0.6f, 0.6f, 0.6f),    //Diffuse
    glm::vec3(1.0f, 1.0f, 1.0f),    //Specular
    1.0f,                           //Constant
    0.0f,                           //Linear
    0.0f},                          //Quadratic

    //Light Ball
    {glm::vec3(ballPos),            //Position
    glm::vec3(0.5f, 0.2f, 0.0f),    //Ambient
    glm::vec3(0.9f, 0.4f, 0.0f),    //Diffuse
    glm::vec3(0.9f, 0.9f, 0.0f),    //Specular
    1.0f,                           //Constant
    1.7f,                           //Linear
    2.8f},                           //Quadratic

    //Light Bar
    {glm::vec3(barPos),            //Position
    glm::vec3(0.0f, 0.2f, 0.5f),    //Ambient
    glm::vec3(0.0f, 0.4f, 0.9f),    //Diffuse
    glm::vec3(0.0f, 0.9f, 0.9f),    //Specular
    1.0f,                           //Constant
    1.9f,                           //Linear
    2.7f}                           //Quadratic
};

//Parametros de frames para el motor
float lastFrame = 0.0f;
float lastX, lastY;
bool firstMouse = true;
//*****************************************************************************************************************************


//Control de input (teclado y raton) ******************************************************************************************
void handleInput(float dt) {
    Input* input = Input::instance();

    /*
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
    */

    //Controles del jugador: movimiento lateral, tecla espacio para mover la bola
    if (input->isKeyPressed(GLFW_KEY_RIGHT)) {
        if(barPos.x <= (BckSize.x /2 -1.0f))
        { 
            barPos += dt * k_barSpeed * glm::vec3(1.0f, 0.0f, 0.0f);
        }
        else
        {
            barPos = barPos;
        }
        
    }
    if (input->isKeyPressed(GLFW_KEY_LEFT)) {

        if (barPos.x >= (-BckSize.x / 2 +1.0f))
        {
            barPos -= dt * k_barSpeed * glm::vec3(1.0f, 0.0f, 0.0f);
        }
        else
        {
            barPos = barPos;
        }
        
    }

    if (input->isKeyPressed(GLFW_KEY_SPACE) && lifes > 0) {
        isPlaying = true;
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

    //camera.handleMouseMovement(xoffset, yoffset);
}

void onScrollMoved(float x, float y) {
    //camera.handleMouseScroll(y);
}
// *****************************************************************************************************************************


//Funciones de pintado de la escena y el juego (Render y RenderScene) *****************************************************************
void renderScene(const Shader& shader,
    const Drawable& drawBck, const Drawable& drawBar, const Drawable& drawBlock, const Drawable& drawWalls, const Drawable& drawBall) {

    //Fondo
    drawBck.setDrawable(shader, glm::vec3(0.0f, 0.0f, -0.5f), 0.0f, glm::vec3(0.0f), glm::vec3(BckSize));

    //Walls
    drawWalls.setDrawable(shader, glm::vec3(BckSize.x/2 +0.25f, 0.0f, -0.5f), 0.0f, glm::vec3(0.0f), glm::vec3(0.5f, 10.0f, 0.75f));
    drawWalls.setDrawable(shader, glm::vec3(-BckSize.x / 2 - 0.25f, 0.0f, -0.5f), 0.0f, glm::vec3(0.0f), glm::vec3(0.5f, 10.0f, 0.75f));
    drawWalls.setDrawable(shader, glm::vec3(0.0f, BckSize.y / 2, -0.5f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.5f, BckSize.x+1.0f, 0.75f));

    //Bar
    drawBar.setDrawable(shader, glm::vec3(barPos), 0.0f, glm::vec3(0.0f), glm::vec3(barSize));

    //Blocks
    for (int j = 0; j < numOfRows; j++) {
        for (int i = 0; i < numOfBlocks; i++) {
            if (!blocks[j][i].getIsDestroyed()) {
                drawBlock.setDrawable(shader, glm::vec3(blocks[j][i].getPosition()), 0.0f, glm::vec3(0.0f), glm::vec3(blocks[j][i].getSize()));
            }
        }
    }

    //Ball
    glm::vec3 rotation;
    if (isPlaying) {
        rotation = glm::vec3(-ballSpeed.x * 90.0f, 0.0f, 0.0f);
    }
    else {
        rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    }
    drawBall.setDrawable(shader, ballPos, static_cast<float>(glfwGetTime()), rotation, glm::vec3(ballRadius));
}

void render(const Shader& s_phong, const Shader& s_depth, const Shader& s_debug, const Shader& s_post,
    const Drawable& drawBck, const Drawable& drawBar, const Drawable& drawBlock, const Drawable& drawWalls, const Drawable& drawBall,
    const uint32_t fbo, const uint32_t fbo_texture) {

    // Sombras
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, k_shadow_width, k_shadow_height);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    const glm::mat4 lightPro = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, k_shadow_near, k_shadow_far);
    const glm::mat4 lightView = glm::lookAt(dirLight.getPosition(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 lightSpaceMatrix = lightPro * lightView;

    s_depth.use();
    s_depth.set("lightSpaceMatrix", lightSpaceMatrix);
    renderScene(s_depth, drawBck, drawBar, drawBlock, drawWalls, drawBall);

    if (lifes > 0) {
        //SECOND PASS: SCREEN
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, Window::instance()->getWidth(), Window::instance()->getHeight());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //camera.setFront(-camera.getPosition());
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 proj = glm::perspective(glm::radians(camera.getFOV()), static_cast<float>(Window::instance()->getWidth()) / Window::instance()->getHeight(), 0.1f, 100.0f);

        s_phong.use();
        s_phong.set("view", view);
        s_phong.set("proj", proj);
        s_phong.set("viewPos", camera.getPosition());

        //Luz direccional
        dirLight.setShader(s_phong);

        //Luz de la bola y luz de la barra
        pointLight[0].setShader(s_phong, 0);
        pointLight[1].setShader(s_phong, 1);
        pointLight[1].setPosition(ballPos);
        pointLight[2].setShader(s_phong, 2);
        pointLight[2].setPosition(barPos);


        //Shadows
        s_phong.set("lightSpaceMatrix", lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, fbo_texture);
        s_phong.set("depthMap", 3);
        renderScene(s_phong, drawBck, drawBar, drawBlock, drawWalls, drawBall);
    }
}
// *************************************************************************************************************************************


// Gestion de colisiones y juego ********************************************************************************************************
//Funcion de gestion inicio y final del juego
bool gameManager()
{
    bool endGame = false;
    if (!isPlaying && lifes > 0) {
        ballPos = glm::vec3(barPos.x, barPos.y + 0.3f, barPos.z);
    }

    if (!isPlaying && lifes == 0) {
        std::cout << "GAME OVER !!!!" << std::endl;
        std::cout << "Score: " << score << std::endl;
        endGame = true;
    }

    if (score >= 105)
    {
        std::cout << "YOU WIN !!!!" << std::endl;
        std::cout << "Score: " << score << std::endl;
        endGame = true;
    }

    if (isPlaying) {
        //Caída de la pelota
        if (ballPos.y < barPos.y - 0.5f) {
            isPlaying = false;
            ballSpeed.y = -1.0 * ballSpeed.y;
            lifes--;
            std::cout << lifes << " vidas" << std::endl;
        }
    }

    return endGame;
}

// Funcion para realizar las colisones de la barra con la bola
void barCollisions()
{
    //Medimos la distancia de la bola a la barra, para ello cogemos su posicion y tenemos en cuenta sus tamaños
    float distBarBall = (ballPos.y - ballRadius) - (barPos.y + barSize.y / 2.0f);
    if (distBarBall < 0.1f) {
        if (ballPos.x <= barPos.x + barSize.x / 2.0f
            && ballPos.x >= barPos.x - barSize.x / 2.0f
            && ballPos.y > barPos.y) {
            ballSpeed.y = ballSpeedY;
        }
    }
}

// Funcion para gestionar las colisiones de la bola con las paredes
void wallsCollisions()
{
    //Limite superior, comparamos su posicion
    if (ballPos.y >= (BckSize.y /2)-0.5f)
    {
        ballSpeed.y = -ballSpeedY;
    }

    //Limites laterales, comparamos su posicion
    //Izquierdo
    if (ballPos.x <= -BckSize.x / 2.0f + ballRadius * 2.0f)
    {
        ballSpeed.x = ballSpeedX;
    }

    //Derecho
    if (ballPos.x >= BckSize.x / 2.0f - ballRadius * 2.0f)
    {
        ballSpeed.x = -ballSpeedX;
    }
}

// Funcion para gestionar las colisiones de los bloques con la bola
void blocksCollisions()
{
    //Recorremos todo la lista de bloques, cada bloque es un tipo de dato creado que contiene su tamaño, su posicion y si esta destruido
    for (int j = 0; j < numOfRows; j++) {
        for (int i = 0; i < numOfBlocks; i++) {
            // Calculamos las distancias de cada borde de bloque respecto a la bola teniendo en cuenta su tamaño
            float bottomDistance = glm::abs((blocks[j][i].getPosition().y - blockSize.y / 2.0f) - (ballPos.y + ballRadius));
            float topDistance = glm::abs((ballPos.y - ballRadius) - (blocks[j][i].getPosition().y + blocks[j][i].getSize().y / 2.0f));
            float rightDistance = glm::abs((ballPos.x - ballRadius) - (blocks[j][i].getPosition().x + blocks[j][i].getSize().x / 2.0f));
            float leftDistance = glm::abs((ballPos.x + ballRadius) - (blocks[j][i].getPosition().x - blocks[j][i].getSize().x / 2.0f));

            //Comparamos con la distancias de los bloques y la bola en cada caso correspondiente
            if (bottomDistance < 0.05f && blocks[j][i].getIsDestroyed() != true) {
                if (ballPos.x <= blocks[j][i].getPosition().x + blocks[j][i].getSize().x / 2.0f && ballPos.x >= blocks[j][i].getPosition().x - blocks[j][i].getSize().x / 2.0f) {
                    ballSpeed.y = -ballSpeedY;
                    blocks[j][i].setIsDestroyed(true);
                    score += 1;

                }

                else if (rightDistance < 0.05f && blocks[j][i].getIsDestroyed() != true) {
                    ballSpeed.x = ballSpeedX;
                    blocks[j][i].setIsDestroyed(true);
                    score += 1;
                }

                else if (leftDistance < 0.05f && blocks[j][i].getIsDestroyed() != true) {
                    ballSpeed.x = -ballSpeedX;
                    blocks[j][i].setIsDestroyed(true);
                    score += 1;
                }
            }
            if (topDistance < 0.05f && blocks[j][i].getIsDestroyed() != true) {
                if (ballPos.x <= blocks[j][i].getPosition().x + blocks[j][i].getSize().x / 2.0f && ballPos.x >= blocks[j][i].getPosition().x - blocks[j][i].getSize().x / 2.0f) {
                    ballSpeed.y = ballSpeedY;
                    blocks[j][i].setIsDestroyed(true);
                    score += 1;
                }

                else if (rightDistance < 0.05f && blocks[j][i].getIsDestroyed() != true) {
                    ballSpeed.x = ballSpeedX;
                    blocks[j][i].setIsDestroyed(true);
                    score += 1;
                }

                else if (leftDistance < 0.05f && blocks[j][i].getIsDestroyed() != true) {
                    ballSpeed.x = -ballSpeedX;
                    blocks[j][i].setIsDestroyed(true);
                    score += 1;
                }
            }

        }
    }
}
// ****************************************************************************************************************************************

// Main con loop
int main(int, char* []) {
    //Creacion de la ventana
    Window* window = Window::instance();
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    //Definicion de Shaders
    const Shader s_phong("../projects/FINAL/phong.vs", "../projects/FINAL/blinn.fs");
    const Shader s_depth("../projects/FINAL/depth.vs", "../projects/FINAL/depth.fs");
    const Shader s_debug("../projects/FINAL/debug.vs", "../projects/FINAL/debug.fs");
    const Shader s_post("../projects/FINAL/fbo.vs", "../projects/FINAL/fbo.fs");

    //Configuracion del fondo
    const Texture t_albedoBck("../assets/textures/bricks_albedo.png", Texture::Format::RGB);
    const Texture t_specularBck("../assets/textures/bricks_specular.png", Texture::Format::RGB);
    const Texture t_normalBck("../assets/textures/bricks_normal.png", Texture::Format::RGB);
    Material materialBck(t_albedoBck, t_specularBck, t_normalBck, 64, s_phong);
    const Quad quad(1.0f);
    Drawable drawBck(quad, materialBck);

    //Configuracion de la Barra
    const Texture t_albedoBar ("../assets/FINAL/textures/bar/diffuse.png", Texture::Format::RGB);
    const Texture t_specularBar ("../assets/FINAL/textures/bar/specular.png", Texture::Format::RGB);
    const Texture t_normalBar("../assets/FINAL/textures/bar/normal.png", Texture::Format::RGB);
    const Cube cube(1.0f);
    Material materialBar(t_albedoBar, t_specularBar, t_normalBar, 64, s_phong);
    Drawable drawBar(cube, materialBar);

    //Configuracion del Bloque
    const Texture t_albedoBlock("../assets/textures/wood.jpg", Texture::Format::RGB);
    const Texture t_specularBlock("../assets/textures/duct.png", Texture::Format::RGB);
    const Texture t_normalBlock("../assets/FINAL/textures/bar/normal.png", Texture::Format::RGB);
    Material materialBlock(t_albedoBlock, t_specularBlock, t_normalBlock, 64, s_phong);
    Drawable drawBlock(cube, materialBlock);

    //Configuracion de la Pared
    const Texture t_albedoWalls("../assets/FINAL/textures/wall/diffuse.png", Texture::Format::RGB);
    const Texture t_specularWalls("../assets/FINAL/textures/wall/specular.png", Texture::Format::RGB);
    const Texture t_normalWalls("../assets/FINAL/textures/wall/normal.png", Texture::Format::RGB);
    Material material_walls(t_albedoWalls, t_specularWalls, t_normalWalls, 64, s_phong);
    Drawable drawWalls(cube, material_walls);

    //Configuracion de la Bola
    const Model model_ball("../assets/FINAL/models/ball/ball.obj");
    const Texture t_albedoBall("../assets/FINAL/models/ball/textures/diffuse.jpg", Texture::Format::RGB);
    const Texture t_specularBall("../assets/FINAL/models/ball/textures/specular.jpg", Texture::Format::RGB);
    const Texture t_normalBall("../assets/FINAL/models/ball/textures/normal.jpg", Texture::Format::RGB);
    Material materialBall(t_albedoBall, t_specularBall, t_normalBall, 64, s_phong);
    Drawable drawBall(model_ball, materialBall);

    //Creacion del FBO 
    auto fbo = fboShadow.createShadowFBO();

    //Colocacion y configuracion de los bloques
    for (int j = 0; j < numOfRows; j++) {
        for (int i = 0; i < numOfBlocks; i++) {
            blocks[j][i].setPosition(glm::vec3(-BckSize.x / 2.0f + (i + 2) * blockSize.x, blockPos.y - j * blockSize.y, 0.0f));
            blocks[j][i].setSize(blockSize);
            blocks[j][i].setIsDestroyed(false);
        }
    }

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    Input::instance()->setKeyPressedCallback(onKeyPress);
    Input::instance()->setMouseMoveCallback(onMouseMoved);
    Input::instance()->setScrollMoveCallback(onScrollMoved);



    while (window->alive()) {
        // Control de los frames
        const float currentFrame = glfwGetTime();
        const float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        handleInput(deltaTime);


        //Pintar los elementos en pantalla
        render(s_phong, s_depth, s_debug, s_post, drawBck, drawBar, drawBlock, drawWalls, drawBall, fbo.first, fbo.second);


        //Movimiento de la bola
        ballPos += glm::vec3(deltaTime * ballSpeed.x, deltaTime * ballSpeed.y, 0.0f);

        //Gestion principal de inicio y final del juego
        bool endGame = gameManager();
        if (endGame) {
            return 0;
        }

        //Gestion de colisiones llamando a las funciones creadas
        barCollisions();
        blocksCollisions();
        wallsCollisions();


        window->frame();
    }

    //Borrado de memoria
    glDeleteFramebuffers(1, &fbo.first);
    glDeleteTextures(1, &fbo.second);

    //Cerrar programa
    return 0;
}

