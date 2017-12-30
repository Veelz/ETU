#include "Cube.h"

Cube::Cube(GLfloat x, GLfloat y, GLfloat z, GLfloat angle_x, GLfloat angle_y, GLfloat angle_z) :
	x(x), y(y), z(z), angle_x(angle_x), angle_y(angle_y), angle_z(angle_z)
{
	this->setMaterial(1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		32.0f);	
	textureVertices = nullptr;
	this->setTextureSize(1.0f);
}

Cube::Cube(glm::vec3 position, glm::vec3 angle) :
	x(position.x), y(position.y), z(position.z), angle_x(angle.x), angle_y(angle.y), angle_z(angle.z)
{
	this->setMaterial(1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		32.0f);
	textureVertices = nullptr;
	this->setTextureSize(1.0f);
}

void Cube::bindTexture(GLuint textureId, GLuint textureType)
{
	if (textureType == GL_TEXTURE_CUBE_MAP) {
		this->cubeMap = textureId;
	}
	else if (textureType == GL_TEXTURE_2D) {
		this->texture = textureId;
	}
}

GLuint Cube::getTexture() const
{
	return this->texture;
}

void Cube::bindVAO(GLuint VAO)
{
	this->VAO = VAO;
}

GLuint Cube::getVAO() const
{
	return this->VAO;
}

void Cube::setMaterial(Material material)
{
	this->material = material;
}

void Cube::setMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLfloat shininess)
{
	this->material = Material{ ambient, diffuse, specular, shininess };
}

void Cube::setMaterial(
	GLfloat ambient_r, GLfloat ambient_g, GLfloat ambient_b, 
	GLfloat diffuse_r, GLfloat diffuse_g, GLfloat diffuse_b, 
	GLfloat specular_r, GLfloat specular_g, GLfloat specular_b, 
	GLfloat shininess)
{
	glm::vec3 ambient(ambient_r, ambient_g, ambient_b);
	glm::vec3 diffuse(diffuse_r, diffuse_g, diffuse_b);
	glm::vec3 specular(specular_r, specular_g, specular_b);
	this->material = Material{ ambient, diffuse, specular, shininess };
}

Material Cube::getMaterial() const
{
	return this->material;
}

void Cube::setTextureSize(GLfloat size)
{
	if (this->textureVertices == nullptr) {
		this->textureVertices = new GLfloat[72];
	}
	for (GLuint i = 0; i < 72; i += 12) {
		textureVertices[i] = textureVertices[i + 11] = 0.0f;
		textureVertices[i + 1] = textureVertices[i + 10] = 0.0f;
		textureVertices[i + 2] = textureVertices[i + 9] = size; // 8.0f;
		textureVertices[i + 3] = textureVertices[i + 8] = 0.0f;
		textureVertices[i + 4] = textureVertices[i + 7] = size; // 8.0f;
		textureVertices[i + 5] = textureVertices[i + 6] = size; // 8.0f;
	}
}

GLfloat * Cube::getTextureVertices()
{
	return this->textureVertices;
}

glm::vec3 Cube::getPosition() const
{
	return glm::vec3(x, y, z);
}

glm::vec3 Cube::getAngle() const
{
	return glm::vec3(angle_x, angle_y, angle_z);
}
GLuint Cube::getTextureType() const
{
	return this->textureType;
}

/*
void Cube::bindRoom(Room * room)
{
	this->room = room;
}
*/
/*
void Cube::render(Shader & shader) const
{
	shader.use();
	glm::mat4 modelMatrix;
	glm::mat4 projectionMatrix = this->room->getProjectionMatrix();
	glm::mat4 viewMatrix = this->room->getViewMatrix();
	glm::mat4 lightSpaceMatrix = this->room->getLightSpaceMatrix();
	Light light = this->room->getLight();

	modelMatrix = glm::translate(modelMatrix, this->getPosition());
	modelMatrix = glm::rotate(modelMatrix, this->getAngle().y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));

	GLuint activeTexture = this->room->getFreeTextureBlock();
	glActiveTexture(activeTexture);
	glBindTexture(this->textureType, this->texture);
	shader.setInt("textureBox", activeTexture);

	shader.setMat4("modelViewProjection", projectionMatrix * viewMatrix * modelMatrix);
	shader.setMat4("modelView", viewMatrix * modelMatrix);
	shader.setMat4("lightModelViewProjection", modelMatrix * lightSpaceMatrix);

	// цвет источника света
	shader.setVec3("light.position", viewMatrix * glm::vec4(light.position, 1.0f));
	shader.setVec3("light.ambient", light.ambient);
	shader.setVec3("light.diffuse", light.diffuse);
	shader.setVec3("light.specular", light.specular);
	// Bronze
	shader.setVec3("material.ambient", this->material.ambient);
	shader.setVec3("material.diffuse", this->material.diffuse);
	shader.setVec3("material.specular", this->material.specular);
	shader.setFloat("material.shininess", this->material.shininess);

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	shader.release();
}
*/