#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>


class Shader {
public:
    // Params
    GLuint program_id;
    
    // Functions
    Shader(const GLchar *vertexPath, const GLchar *fragmentPath);
    void use() const;
	void release() const;
    void setMat4(const GLchar *path, glm::mat4 matrix) const;
    void setVec3(const GLchar *path, GLfloat x, GLfloat y, GLfloat z) const;
    void setVec3(const GLchar *path, glm::vec3 vector) const;
    void setFloat(const GLchar *path, GLfloat n) const;
	void setInt(const GLchar *path, GLint n) const;
};
