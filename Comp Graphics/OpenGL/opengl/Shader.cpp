#include "Shader.h"

Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath) {
    std::string vShaderSourceCode;
    std::string fShaderSourceCode;
    std::ifstream vShaderSourceFile;
    std::ifstream fShaderSourceFile;

    try {
        vShaderSourceFile.open(vertexPath);
        fShaderSourceFile.open(fragmentPath);
        
        std::stringstream vertexStream, fragmentStream;

        vertexStream << vShaderSourceFile.rdbuf();
        fragmentStream << fShaderSourceFile.rdbuf();

        vShaderSourceFile.close();
        fShaderSourceFile.close();

        vShaderSourceCode = vertexStream.str();
        fShaderSourceCode = fragmentStream.str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const const GLchar *vShaderCode = vShaderSourceCode.c_str();
    const const GLchar *fShaderCode = fShaderSourceCode.c_str();

    GLuint vertexShader, fragmentShader;
    GLint success;
    GLchar infoLog[512];
    
    // создание и компил€ци€ вершинного шейдера
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);

    // проверка успешности сборки шейдера
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // создание и компил€ци€ фрагментного шейдера
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);

    // проверка успешности сборки шейдера
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // создание шейдерной программы
    this->program_id = glCreateProgram();
    glAttachShader(this->program_id, vertexShader);
    glAttachShader(this->program_id, fragmentShader);
    glLinkProgram(this->program_id);

    // проверка успешности сборки программы
    glGetProgramiv(this->program_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->program_id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() const {
    glUseProgram(this->program_id);
}

void Shader::release() const
{
	glUseProgram(0);
}

void Shader::setMat4(const GLchar * path, glm::mat4 matrix) const
{
    GLint location = glGetUniformLocation(this->program_id, path);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVec3(const GLchar * path, GLfloat x, GLfloat y, GLfloat z) const
{
    glUniform3f(glGetUniformLocation(this->program_id, path), x, y, z);
}

void Shader::setVec3(const GLchar * path, glm::vec3 vector) const
{
    glUniform3f(glGetUniformLocation(this->program_id, path), (GLfloat)vector.x, (GLfloat)vector.y, (GLfloat)vector.z);
}

void Shader::setFloat(const GLchar * path, GLfloat n) const
{
    glUniform1f(glGetUniformLocation(this->program_id, path), n);
}

void Shader::setInt(const GLchar * path, GLint n) const
{
	glUniform1i(glGetUniformLocation(this->program_id, path), n);
}
