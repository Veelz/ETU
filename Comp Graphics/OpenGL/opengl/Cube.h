#pragma once

#include <vector>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Shader.h"
// #include "Room.h"

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat shininess;
};

class Cube {
public:
	Cube(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0, GLfloat angle_x = 0, GLfloat angle_y = 0, GLfloat angle_z = 0);
	Cube(glm::vec3 position, glm::vec3 angle = glm::vec3(0.0f, 0.0f, 0.0f));
	
	void bindTexture(GLuint textureId, GLuint textureType);
	GLuint getTexture() const;

	void bindVAO(GLuint VAO);
	GLuint getVAO() const;
	
	void setMaterial(Material material);
	void setMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLfloat shininess);
	void setMaterial(
		GLfloat ambient_r, GLfloat ambient_g, GLfloat ambient_b,
		GLfloat diffuse_r, GLfloat diffuse_g, GLfloat diffuse_b,
		GLfloat specular_r, GLfloat specular_g, GLfloat specular_b,
		GLfloat shininess);
	Material getMaterial() const;

	void setTextureSize(GLfloat size);
	GLfloat * getTextureVertices();

	glm::vec3 getPosition() const;
	glm::vec3 getAngle() const;

	GLuint getTextureType() const;

private: 
	GLfloat *textureVertices;
	GLfloat x, y, z, angle_x, angle_y, angle_z;
	GLuint texture;
	GLuint cubeMap;
	Material material;
	GLuint VAO;
	GLuint textureType;
};