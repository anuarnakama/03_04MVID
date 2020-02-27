#include "engine/light.hpp"
#include "engine/shader.hpp"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	: _direction(direction), _position(position), _ambient(ambient), _diffuse(diffuse), _specular(specular) {
}

glm::vec3 DirectionalLight::getPosition() {
	return _position;
}

glm::vec3 DirectionalLight::getDirection() {
	return _direction;
}

void DirectionalLight::setShader(const Shader& shader) {
	shader.set("dirLight.direction", _direction);
	shader.set("dirLight.position", _position);
	shader.set("dirLight.ambient", _ambient);
	shader.set("dirLight.diffuse", _diffuse);
	shader.set("dirLight.specular", _specular);
}


PointLight::PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic)
	: _position(position), _ambient(ambient), _diffuse(diffuse), _specular(specular), _constant(constant), _linear(linear), _quadratic(quadratic) {
}

PointLight::PointLight() {
}

glm::vec3 PointLight::getPosition() {
	return _position;
}

glm::vec3 PointLight::getColor() {
	return _diffuse;
}

void PointLight::setShader(const Shader& shader, uint32_t unit) {
	std::string num = std::to_string(unit);
	shader.set(("pointLight[" + num + "].position").c_str(), _position);
	shader.set(("pointLight[" + num + "].ambient").c_str(), _ambient);
	shader.set(("pointLight[" + num + "].diffuse").c_str(), _diffuse);
	shader.set(("pointLight[" + num + "].specular").c_str(), _specular);
	shader.set(("pointLight[" + num + "].constant").c_str(), _constant);
	shader.set(("pointLight[" + num + "].linear").c_str(), _linear);
	shader.set(("pointLight[" + num + "].quadratic").c_str(), _quadratic);
}

void PointLight::setPosition(glm::vec3 position) {
	_position = position;
}


SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff)
	: _position(position), _direction(direction), _ambient(ambient), _diffuse(diffuse), _specular(specular), _constant(constant), _linear(linear), _quadratic(quadratic), _cutOff(cutOff), _outerCutOff(outerCutOff) {
}


glm::vec3 SpotLight::getPosition() {
	return _position;
}

glm::vec3 SpotLight::getColor() {
	return _diffuse;
}

void SpotLight::setShader(const Shader& shader, uint32_t unit) {
	std::string num = std::to_string(unit);
	shader.set(("spotLight[" + num + "].position").c_str(), _position);
	shader.set(("spotLight[" + num + "].direction").c_str(), _direction);
	shader.set(("spotLight[" + num + "].ambient").c_str(), _ambient);
	shader.set(("spotLight[" + num + "].diffuse").c_str(), _diffuse);
	shader.set(("spotLight[" + num + "].specular").c_str(), _specular);
	shader.set(("spotLight[" + num + "].constant").c_str(), _constant);
	shader.set(("spotLight[" + num + "].linear").c_str(), _linear);
	shader.set(("spotLight[" + num + "].quadratic").c_str(), _quadratic);
	shader.set(("spotLight[" + num + "].cutOff").c_str(), _cutOff);
	shader.set(("spotLight[" + num + "].outerCutOff").c_str(), _outerCutOff);
}